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

#pragma ident	"@(#)mlib_v_ImageConv5x5_8nw.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_conv5x5_8nw.c - convolve an 8-bit image with a 5x5 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *      mlib_status mlib_conv5x5nw_u8(mlib_image       *src,
 *                                    const mlib_image *dst,
 *                                    mlib_s32         *kernel,
 *                                    mlib_s32         scalef_expon,
 *                                    mlib_s32         cmask)
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
 *      A 2-D convolution (5x5 kernel) for 8-bit images.
 */

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
#include <mlib_v_ImageConv.h>

/* *********************************************************** */

static mlib_status mlib_v_conv5x5_8nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_conv5x5_8nw_mask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

/* *********************************************************** */

static const mlib_s32 mlib_round_8[16] = {
	0x00400040, 0x00200020, 0x00100010, 0x00080008,
	0x00040004, 0x00020002, 0x00010001, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS()                                \
	adr_src = mlib_ImageGetData(src);                       \
	adr_dst = mlib_ImageGetData(dst);                       \
	slb = mlib_ImageGetStride(src);                         \
	dlb = mlib_ImageGetStride(dst);                         \
	dh = mlib_ImageGetHeight(dst);                          \
	dw = mlib_ImageGetWidth(dst)

/* *********************************************************** */

#define	LOAD_KERNEL_INTO_FLOAT()                                         \
	k1k2 =                                                           \
	vis_to_float((kernel[0] & 0xFFFF0000) | ((kernel[1] >> 16) &     \
	    0xFFFF));                                                    \
	k3k4 =                                                           \
	vis_to_float((kernel[2] & 0xFFFF0000) | ((kernel[3] >> 16) &     \
	    0xFFFF));                                                    \
	k5k6 =                                                           \
	vis_to_float((kernel[4] & 0xFFFF0000) | ((kernel[5] >> 16) &     \
	    0xFFFF));                                                    \
	k7k8 =                                                           \
	vis_to_float((kernel[6] & 0xFFFF0000) | ((kernel[7] >> 16) &     \
	    0xFFFF));                                                    \
	k9k10 =                                                          \
	vis_to_float((kernel[8] & 0xFFFF0000) | ((kernel[9] >> 16) &     \
	    0xFFFF));                                                    \
	k11k12 =                                                         \
	vis_to_float((kernel[10] & 0xFFFF0000) | ((kernel[11] >> 16) &   \
	    0xFFFF));                                                    \
	k13k14 =                                                         \
	vis_to_float((kernel[12] & 0xFFFF0000) | ((kernel[13] >> 16) &   \
	    0xFFFF));                                                    \
	k15k16 =                                                         \
	vis_to_float((kernel[14] & 0xFFFF0000) | ((kernel[15] >> 16) &   \
	    0xFFFF));                                                    \
	k17k18 =                                                         \
	vis_to_float((kernel[16] & 0xFFFF0000) | ((kernel[17] >> 16) &   \
	    0xFFFF));                                                    \
	k19k20 =                                                         \
	vis_to_float((kernel[18] & 0xFFFF0000) | ((kernel[19] >> 16) &   \
	    0xFFFF));                                                    \
	k21k22 =                                                         \
	vis_to_float((kernel[20] & 0xFFFF0000) | ((kernel[21] >> 16) &   \
	    0xFFFF));                                                    \
	k23k24 =                                                         \
	vis_to_float((kernel[22] & 0xFFFF0000) | ((kernel[23] >> 16) &   \
	    0xFFFF));                                                    \
	k25 =                                                            \
	vis_to_float((kernel[24] & 0xFFFF0000) | ((kernel[24] >> 16) &   \
	    0xFFFF))

/* *********************************************************** */

#define	PREPARE_INTERM_BUFFERS()                                          \
	buff_src =                                                        \
	    (mlib_d64 *)__mlib_malloc(7 * buf_slb * sizeof (mlib_d64));   \
	if (buff_src == NULL)                                             \
	    return (MLIB_FAILURE);                                        \
	sbuf1 = buff_src;                                                 \
	sbuf2 = sbuf1 + buf_slb;                                          \
	sbuf3 = sbuf2 + buf_slb;                                          \
	sbuf4 = sbuf3 + buf_slb;                                          \
	sbuf5 = sbuf4 + buf_slb;                                          \
	dbuf = sbuf5 + buf_slb

/* *********************************************************** */

#define	PREPARE_TO_LOAD_LINE(p_buf, p_line)                     \
	prow = p_buf;                                           \
	dsa = (mlib_d64 *)vis_alignaddr(p_line, 0);             \
	sd1 = dsa[0];                                           \
	dsa++

/* *********************************************************** */

#define	LOAD_LINE_INTO_BUFFER(n)                                \
	for (i = 0; i < (dw + n); i += 8) {                     \
	    sd0 = sd1;                                          \
	    sd1 = dsa[0];                                       \
	    (*prow++) = vis_faligndata(sd0, sd1);               \
	    dsa++;                                              \
	}

/* *********************************************************** */

#define	LOAD_LINE_INTO_BUFFER_NF(n)                             \
	for (i = 0; i < (dw + n); i += 8) {                     \
	    sd0 = sd1;                                          \
	    sd1 = vis_ld_d64_nf(dsa);                           \
	    (*prow++) = vis_faligndata(sd0, sd1);               \
	    dsa++;                                              \
	}

/* *********************************************************** */

#define	LOOP_INI()                                              \
	ddst = (mlib_d64 *)dbuf;                                \
	da = d_a;                                               \
	dend = da + (dw - 1);                                   \
	prow = sbuf1;                                           \
	sbuf1 = sbuf2;                                          \
	sbuf2 = sbuf3;                                          \
	sbuf3 = sbuf4;                                          \
	sbuf4 = sbuf5;                                          \
	sbuf5 = prow;                                           \
	s1 = (mlib_d64 *)sbuf1;                                 \
	s2 = (mlib_d64 *)sbuf2;                                 \
	s3 = (mlib_d64 *)sbuf3;                                 \
	s4 = (mlib_d64 *)sbuf4;                                 \
	s5 = (mlib_d64 *)sbuf5

/* *********************************************************** */

#define	PREPARE_TO_COPY_INTERM_BUF_TO_DST()                     \
	ddst = dbuf;                                            \
/*                                                              \
 * prepare the destination addresses                            \
 */                                                             \
	dp = (mlib_d64 *)((mlib_addr)da & (~7));                \
	i = (mlib_addr)dp - (mlib_addr)da;                      \
	ddst = vis_alignaddr(ddst, i);                          \
/*                                                              \
 * generate edge mask for the start point                       \
 */                                                             \
	emask = vis_edge8(da, dend);                            \
	sd1 = ddst[0];                                          \
	if (emask != 0xff) {                                    \
	    sd0 = sd1;                                          \
	    sd1 = ddst[1];                                      \
	    sd0 = vis_faligndata(sd0, sd1);                     \
	    vis_pst_8(sd0, dp++, emask);                        \
	    ddst++;                                             \
	    i += 8;                                             \
	}

/* *********************************************************** */

#define	COPY_INTERM_BUF_TO_DST()                                \
	for (; i <= (dw - 8); i += 8) {                         \
	    sd0 = sd1;                                          \
	    sd1 = ddst[1];                                      \
	    (*dp++) = vis_faligndata(sd0, sd1);                 \
	    ddst++;                                             \
	}

/* *********************************************************** */

#define	COPY_TAIL()                                             \
	if (i < dw) {                                           \
	    sd0 = vis_faligndata(sd1, ddst[1]);                 \
	    emask = vis_edge8(dp, dend);                        \
	    vis_pst_8(sd0, dp, emask);                          \
	}

/* *********************************************************** */

#define	CONV_AU(dsrc, ka)                                       \
	tmp0 = vis_fmul8x16au(vis_read_hi(dsrc), ka);           \
	tmp1 = vis_fmul8x16au(vis_read_lo(dsrc), ka);           \
	out0 = vis_fpadd16(out0, tmp0);                         \
	out1 = vis_fpadd16(out1, tmp1)

/* *********************************************************** */

#define	CONV_AL(dsrc, ka)                                       \
	tmp0 = vis_fmul8x16al(vis_read_hi(dsrc), ka);           \
	tmp1 = vis_fmul8x16al(vis_read_lo(dsrc), ka);           \
	out0 = vis_fpadd16(out0, tmp0);                         \
	out1 = vis_fpadd16(out1, tmp1)

/* *********************************************************** */

#define	SHIFT_U8_1                                              \
	d21 = vis_faligndata(d21, dt_1);                        \
	dt_1 = vis_faligndata(dt_1, d1);                        \
	d22 = vis_faligndata(d22, dt_2);                        \
	dt_2 = vis_faligndata(dt_2, d2);                        \
	d23 = vis_faligndata(d23, dt_3);                        \
	dt_3 = vis_faligndata(dt_3, d3)

/* *********************************************************** */

#define	SHIFT_U8_2                                              \
	d24 = vis_faligndata(d24, dt_4);                        \
	dt_4 = vis_faligndata(dt_4, d4);                        \
	d25 = vis_faligndata(d25, dt_5);                        \
	dt_5 = vis_faligndata(dt_5, d5)

/* *********************************************************** */

mlib_status
mlib_v_conv5x5_8nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf3, *sbuf4, *sbuf5;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf, *dbuf1;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* data */
	mlib_d64 d1, d2, d3, d4, d5;

/* data */
	mlib_d64 d11, d12, d13, d14, d15;

/* data */
	mlib_d64 d21, d22, d23, d24, d25;

/* data */
	mlib_d64 dt_1, dt_2, dt_3, dt_4, dt_5;
	mlib_f32 k1k2, k3k4, k5k6, k7k8;
	mlib_f32 k9k10, k11k12, k13k14, k15k16;
	mlib_f32 k17k18, k19k20, k21k22, k23k24, k25;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1;
	mlib_d64 tmp0, tmp1, rnd;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 emask;
	mlib_s32 rval, gsr_scale, i, j;

	gsr_scale = 31 - scalef_expon;
	vis_write_gsr((gsr_scale << 3));
	rval = mlib_round_8[gsr_scale];
	rnd = vis_freg_pair(vis_to_float(rval), vis_to_float(rval));

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	buf_slb = (4 * dw + 24) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 4;
	dw *= 4;
	dh -= 4;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	sa4 = sa3 + slb;
	d_a = adr_dst + 2 * dlb + 8;

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(16);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(16);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(16);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf5, sa3);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(16);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf5, sa4);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(16);

		vis_alignaddr(s1, 4);
		dbuf1 = dbuf;
		d1 = *s1;
		d2 = *s2;
		d3 = *s3;
		d11 = *(s1 + 1);
		d12 = *(s2 + 1);
		d13 = *(s3 + 1);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d21 = *(s1 + 2);
			d22 = *(s2 + 2);
			d23 = *(s3 + 2);
			out0 = out1 = rnd;
			CONV_AU(d1, k1k2);
			CONV_AL(d2, k5k6);
			CONV_AU(d3, k11k12);
			dt_1 = vis_faligndata(d1, d11);
			dt_2 = vis_faligndata(d2, d12);
			dt_3 = vis_faligndata(d3, d13);
			CONV_AL(dt_1, k1k2);
			CONV_AU(dt_2, k7k8);
			CONV_AL(dt_3, k11k12);
			CONV_AU(d11, k3k4);
			CONV_AL(d12, k7k8);
			CONV_AU(d13, k13k14);
			dt_1 = vis_faligndata(d11, d21);
			dt_2 = vis_faligndata(d12, d22);
			dt_3 = vis_faligndata(d13, d23);
			CONV_AL(dt_1, k3k4);
			CONV_AU(dt_2, k9k10);
			CONV_AL(dt_3, k13k14);
			CONV_AU(d21, k5k6);
			CONV_AL(d22, k9k10);
			CONV_AU(d23, k15k16);
			dbuf1[0] = out0;
			dbuf1[1] = out1;
			dbuf1 += 2;
			d1 = d11;
			d2 = d12;
			d3 = d13;
			d11 = d21;
			d12 = d22;
			d13 = d23;
			s1++;
			s2++;
			s3++;
		}

		dbuf1 = dbuf;
		d4 = *s4;
		d5 = *s5;
		d14 = *(s4 + 1);
		d15 = *(s5 + 1);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d24 = *(s4 + 2);
			d25 = *(s5 + 2);
			out0 = dbuf1[0];
			out1 = dbuf1[1];
			CONV_AL(d4, k15k16);
			CONV_AU(d5, k21k22);
			dt_4 = vis_faligndata(d4, d14);
			dt_5 = vis_faligndata(d5, d15);
			CONV_AU(dt_4, k17k18);
			CONV_AL(dt_5, k21k22);
			CONV_AL(d14, k17k18);
			CONV_AU(d15, k23k24);
			dt_4 = vis_faligndata(d14, d24);
			dt_5 = vis_faligndata(d15, d25);
			CONV_AU(dt_4, k19k20);
			CONV_AL(dt_5, k23k24);
			CONV_AL(d24, k19k20);
			CONV_AU(d25, k25);
			dbuf1 += 2;
			(*ddst++) = vis_fpack16_pair(out0, out1);
			d4 = d14;
			d5 = d15;
			d14 = d24;
			d15 = d25;
			s4++;
			s5++;
		}

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();

		sa4 = sa4 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_conv5x5_8nw_mask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *dend, *adr_src;

/* pointers to src rows */
	mlib_u8 *sa, *sa2, *sa3, *sa4, *sa5, *sa6, *sa_6, *prow;

/* pointers to rows in interm. src buf */
	mlib_u8 *buff_src, *sbuf1, *sbuf2, *sbuf3, *sbuf4, *sbuf5, *s_buf1;

/* pointers to row in interm. dst buf */
	mlib_u8 *dbuf, *d_buf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst, *ddst1;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, rnd;

/* data */
	mlib_d64 d1, d2, d3, d4, d5, d_1, d_2, d_3, d_4, d_5;

/* temp. data, used in faligndata */
	mlib_d64 dt_1, dt_2, dt_3, dt_4, dt_5;

/* shifted data */
	mlib_d64 d21, d22, d23, d24, d25;
	mlib_f32 k1k2, k17k18, k19k20, k21k22, k23k24, k25;
	mlib_f32 k3k4, k5k6, k7k8, k9k10, k11k12, k13k14, k15k16;
	mlib_s32 rval, gsr_scale, i, j, nchannel, nchannel1, chan, testchan;

/* temp, used in load-store */
	mlib_s32 t1, t2, t3, t4, t5, t6, t7, t8, tt1, tt2, tt3, tt4, tt5, tt6,
	    tt7, tt8;

	adr_src = mlib_ImageGetData(src);
	adr_dst = mlib_ImageGetData(dst);
	nchannel = mlib_ImageGetChannels(src);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	dh = mlib_ImageGetHeight(dst);
	dw = mlib_ImageGetWidth(dst);

/* buf_slb - 8-byte aligned */
	buf_slb = (dw + 16) & (~7);
/* alloc. interm. src and dst buffer */
	buff_src = (mlib_u8 *)__mlib_malloc(7 * buf_slb * sizeof (mlib_u8) + 8);

	if (buff_src == NULL)
		return (MLIB_FAILURE);
/* edge - no write */
	dw -= 4;
	dh -= 4;

/*
 * The 8x16 mult has built-in 8-bit R shift, and fpack16 has 7-bit
 * fixed R shift (preceded by variable-bit L shift controlled by GSR
 * scalefactor field). Thus net R shift = (8+7)-(GSR.scalefactor_field),
 * so GSR.scalefactor_field = 15-(net R shift):
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
	dbuf = sbuf5 + buf_slb;

	LOAD_KERNEL_INTO_FLOAT();

	testchan = 1;

	for (chan = nchannel - 1; chan >= 0; chan--) {
		if ((cmask & testchan) == 0) {
			testchan <<= 1;
			continue;
		}

		testchan <<= 1;
		sa = adr_src + chan;
		sa2 = sa + slb;
		sa3 = sa2 + slb;
		sa4 = sa3 + slb;
		sa5 = sa4 + slb;
		sa_6 = sa6 = sa5 + slb;
		d_a = adr_dst + (dlb << 1) + (nchannel << 1) + chan;

/* load interm. src buff */
		for (i = 0, j = 0; j < (dw + 4); i += nchannel, j++) {
			sbuf1[j] = sa5[i];
			sbuf2[j] = sa[i];
			sbuf3[j] = sa2[i];
			sbuf4[j] = sa3[i];
			sbuf5[j] = sa4[i];
		}

		for (j = 0; j < dh - 1; j++) {
			ddst1 = ddst = (mlib_d64 *)(dbuf);
			d_buf = (dbuf - 8);
			da = d_a;
			dend = da + (dw - 1) * nchannel;
			prow = sbuf1;
			sbuf1 = sbuf2;
			sbuf2 = sbuf3;
			sbuf3 = sbuf4;
			sbuf4 = sbuf5;
			sbuf5 = prow;
			s1 = (mlib_d64 *)sbuf1;
			s2 = (mlib_d64 *)sbuf2;
			s3 = (mlib_d64 *)sbuf3;
			s4 = (mlib_d64 *)sbuf4;
			s5 = (mlib_d64 *)sbuf5;
			s_buf1 = sbuf1;
			d1 = *s1;
			d2 = *s2;
			d3 = *s3;
			nchannel1 = 0;

#pragma pipeloop(0)
			for (i = 0; i < dw; i += 8) {
				d_1 = *(s1 + 1);
				d_2 = *(s2 + 1);
				d_3 = *(s3 + 1);
				out0 = out1 = rnd;
				t1 = vis_ld_u8_nf(sa_6);
				sa_6 += nchannel;
				CONV_AU(d1, k1k2);
				t2 = vis_ld_u8_nf(sa_6);
				sa_6 += nchannel;
				CONV_AL(d2, k5k6);
				t3 = vis_ld_u8_nf(sa_6);
				sa_6 += nchannel;
				CONV_AU(d3, k11k12);
				t4 = vis_ld_u8_nf(sa_6);
				sa_6 += nchannel;
				d21 = vis_faligndata(d1, d_1);
				dt_1 = vis_faligndata(d_1, d1);
				t5 = vis_ld_u8_nf(sa_6);
				sa_6 += nchannel;
				d22 = vis_faligndata(d2, d_2);
				dt_2 = vis_faligndata(d_2, d2);
				t6 = vis_ld_u8_nf(sa_6);
				sa_6 += nchannel;
				d23 = vis_faligndata(d3, d_3);
				t7 = vis_ld_u8_nf(sa_6);
				sa_6 += nchannel;
				dt_3 = vis_faligndata(d_3, d3);
				t8 = vis_ld_u8_nf(sa_6);
				sa_6 += nchannel;
				CONV_AL(d21, k1k2);
				(*s_buf1++) = t1;
				CONV_AU(d22, k7k8);
				(*s_buf1++) = t2;
				CONV_AL(d23, k11k12);
				(*s_buf1++) = t3;
				SHIFT_U8_1;
				CONV_AU(d21, k3k4);
				(*s_buf1++) = t4;
				CONV_AL(d22, k7k8);
				CONV_AU(d23, k13k14);
				d21 = vis_faligndata(d21, dt_1);
				d22 = vis_faligndata(d22, dt_2);
				(*s_buf1++) = t5;
				d23 = vis_faligndata(d23, dt_3);
				CONV_AL(d21, k3k4);
				(*s_buf1++) = t6;
				CONV_AU(d22, k9k10);
				(*s_buf1++) = t7;
				CONV_AL(d23, k13k14);
				d21 =
				    vis_freg_pair(vis_read_lo(d1),
				    vis_read_hi(d_1));
				CONV_AU(d21, k5k6);
				d22 =
				    vis_freg_pair(vis_read_lo(d2),
				    vis_read_hi(d_2));
				CONV_AL(d22, k9k10);
				d23 =
				    vis_freg_pair(vis_read_lo(d3),
				    vis_read_hi(d_3));
				CONV_AU(d23, k15k16);
				(*s_buf1++) = t8;
				ddst[0] = out0;
				ddst[1] = out1;
				ddst += 2;
				d1 = d_1;
				d2 = d_2;
				d3 = d_3;
				s1++;
				s2++;
				s3++;
			}

			ddst = (mlib_d64 *)(dbuf);
			d4 = *s4;
			d5 = *s5;
/*
 * in each iteration store result from prev. iterat.
 * and load data for processing next row
 */
#pragma pipeloop(0)
			for (i = 0; i < dw; i += 8) {
				d_4 = *(s4 + 1);
				d_5 = *(s5 + 1);
				out0 = ddst[0];
				out1 = ddst[1];
				ddst += 2;
				tt1 = (*d_buf++);
				CONV_AL(d4, k15k16);
				tt2 = (*d_buf++);
				CONV_AU(d5, k21k22);
				d24 = vis_faligndata(d4, d_4);
				tt3 = (*d_buf++);
				dt_4 = vis_faligndata(d_4, d4);
				d25 = vis_faligndata(d5, d_5);
				tt4 = (*d_buf++);
				dt_5 = vis_faligndata(d_5, d5);
				tt5 = (*d_buf++);
				CONV_AU(d24, k17k18);
				tt6 = (*d_buf++);
				CONV_AL(d25, k21k22);
				tt7 = (*d_buf++);
				SHIFT_U8_2;
				tt8 = (*d_buf++);
				CONV_AL(d24, k17k18);
				*da = tt1;
				da += nchannel1;
				CONV_AU(d25, k23k24);
				*da = tt2;
				da += nchannel1;
				d24 = vis_faligndata(d24, dt_4);
				*da = tt3;
				da += nchannel1;
				d25 = vis_faligndata(d25, dt_5);
				*da = tt4;
				da += nchannel1;
				CONV_AU(d24, k19k20);
				*da = tt5;
				da += nchannel1;
				CONV_AL(d25, k23k24);
				*da = tt6;
				da += nchannel1;
				d24 =
				    vis_freg_pair(vis_read_lo(d4),
				    vis_read_hi(d_4));
				CONV_AL(d24, k19k20);
				*da = tt7;
				da += nchannel1;
				d25 =
				    vis_freg_pair(vis_read_lo(d5),
				    vis_read_hi(d_5));
				CONV_AU(d25, k25);
				*da = tt8;
				da += nchannel1;
				(*ddst1++) = vis_fpack16_pair(out0, out1);
				d4 = d_4;
				d5 = d_5;
				s4++;
				s5++;
				nchannel1 = nchannel;
			}

			(*s_buf1++) = vis_ld_u8_nf(sa_6);
			sa_6 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_6);
			sa_6 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_6);
			sa_6 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_6);
			sa_6 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_6);
			sa_6 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_6);
			sa_6 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_6);
			sa_6 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_6);

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = (*d_buf++);
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = (*d_buf++);
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = (*d_buf++);
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = (*d_buf++);
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = (*d_buf++);
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = (*d_buf++);
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = (*d_buf++);
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = (*d_buf++);
			}

			sa_6 = sa6 = sa6 + slb;
			d_a += dlb;
		}

/* process last row - no need to load data */
		ddst1 = ddst = (mlib_d64 *)(dbuf);
		d_buf = (dbuf - 8);
		da = d_a;
		dend = da + (dw - 1) * nchannel;
		prow = sbuf1;
		sbuf1 = sbuf2;
		sbuf2 = sbuf3;
		sbuf3 = sbuf4;
		sbuf4 = sbuf5;
		sbuf5 = prow;
		s1 = (mlib_d64 *)sbuf1;
		s2 = (mlib_d64 *)sbuf2;
		s3 = (mlib_d64 *)sbuf3;
		s4 = (mlib_d64 *)sbuf4;
		s5 = (mlib_d64 *)sbuf5;
		d1 = *s1;
		d2 = *s2;
		d3 = *s3;
		nchannel1 = 0;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_3 = *(s3 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k1k2);
			CONV_AL(d2, k5k6);
			CONV_AU(d3, k11k12);
			d21 = vis_faligndata(d1, d_1);
			dt_1 = vis_faligndata(d_1, d1);
			d22 = vis_faligndata(d2, d_2);
			dt_2 = vis_faligndata(d_2, d2);
			d23 = vis_faligndata(d3, d_3);
			dt_3 = vis_faligndata(d_3, d3);
			CONV_AL(d21, k1k2);
			CONV_AU(d22, k7k8);
			CONV_AL(d23, k11k12);
			SHIFT_U8_1;
			CONV_AU(d21, k3k4);
			CONV_AL(d22, k7k8);
			CONV_AU(d23, k13k14);
			d21 = vis_faligndata(d21, dt_1);
			d22 = vis_faligndata(d22, dt_2);
			d23 = vis_faligndata(d23, dt_3);
			CONV_AL(d21, k3k4);
			CONV_AU(d22, k9k10);
			CONV_AL(d23, k13k14);
			d21 = vis_freg_pair(vis_read_lo(d1), vis_read_hi(d_1));
			CONV_AU(d21, k5k6);
			d22 = vis_freg_pair(vis_read_lo(d2), vis_read_hi(d_2));
			CONV_AL(d22, k9k10);
			d23 = vis_freg_pair(vis_read_lo(d3), vis_read_hi(d_3));
			CONV_AU(d23, k15k16);
			ddst[0] = out0;
			ddst[1] = out1;
			ddst += 2;
			d1 = d_1;
			d2 = d_2;
			d3 = d_3;
			s1++;
			s2++;
			s3++;
		}

		ddst = (mlib_d64 *)(dbuf);
		d4 = *s4;
		d5 = *s5;
/*
 * in each iteration store result from prev. iterat.
 * and load data for processing next row
 */
#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_4 = *(s4 + 1);
			d_5 = *(s5 + 1);
			out0 = ddst[0];
			out1 = ddst[1];
			ddst += 2;
			tt1 = (*d_buf++);
			CONV_AL(d4, k15k16);
			tt2 = (*d_buf++);
			CONV_AU(d5, k21k22);
			d24 = vis_faligndata(d4, d_4);
			tt3 = (*d_buf++);
			dt_4 = vis_faligndata(d_4, d4);
			d25 = vis_faligndata(d5, d_5);
			tt4 = (*d_buf++);
			dt_5 = vis_faligndata(d_5, d5);
			tt5 = (*d_buf++);
			CONV_AU(d24, k17k18);
			tt6 = (*d_buf++);
			CONV_AL(d25, k21k22);
			tt7 = (*d_buf++);
			SHIFT_U8_2;
			tt8 = (*d_buf++);
			CONV_AL(d24, k17k18);
			*da = tt1;
			da += nchannel1;
			CONV_AU(d25, k23k24);
			*da = tt2;
			da += nchannel1;
			d24 = vis_faligndata(d24, dt_4);
			*da = tt3;
			da += nchannel1;
			d25 = vis_faligndata(d25, dt_5);
			*da = tt4;
			da += nchannel1;
			CONV_AU(d24, k19k20);
			*da = tt5;
			da += nchannel1;
			CONV_AL(d25, k23k24);
			*da = tt6;
			da += nchannel1;
			d24 = vis_freg_pair(vis_read_lo(d4), vis_read_hi(d_4));
			CONV_AL(d24, k19k20);
			*da = tt7;
			da += nchannel1;
			d25 = vis_freg_pair(vis_read_lo(d5), vis_read_hi(d_5));
			CONV_AU(d25, k25);
			*da = tt8;
			da += nchannel1;
			(*ddst1++) = vis_fpack16_pair(out0, out1);
			d4 = d_4;
			d5 = d_5;
			s4++;
			s5++;
			nchannel1 = nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = (*d_buf++);
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = (*d_buf++);
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = (*d_buf++);
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = (*d_buf++);
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = (*d_buf++);
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = (*d_buf++);
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = (*d_buf++);
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = (*d_buf++);
		}
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_conv5x5nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchan, amask;

	if (mlib_ImageConvVersion(5, 5, scalef_expon, MLIB_BYTE) == 0)
		return mlib_c_conv5x5nw_u8(dst, src, kernel, scalef_expon,
		    cmask);

	nchan = mlib_ImageGetChannels(src);
	amask = (1 << nchan) - 1;

	if (((cmask & amask) == amask) && (nchan > 1)) {
		if (nchan == 4) {
			return mlib_v_conv5x5_8nw_4(dst, src, kernel,
			    scalef_expon);
		} else {
			return mlib_convMxN_8nw_f(dst, src, 5, 5, 2, 2, kernel,
			    scalef_expon);
		}
	}

	return (mlib_v_conv5x5_8nw_mask(dst, src, kernel, scalef_expon, cmask));
}

/* *********************************************************** */
