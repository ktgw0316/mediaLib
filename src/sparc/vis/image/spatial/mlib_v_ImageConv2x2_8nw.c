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

#pragma ident	"@(#)mlib_v_ImageConv2x2_8nw.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_conv2x2_8nw.c - convolve an 8-bit image with a 2x2 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *      mlib_status mlib_conv2x2nw_u8(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *kernel,
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
 *      A 2-D convolution (2x2 kernel) for 8-bit images.
 */

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
#include <mlib_v_ImageConv.h>

/* *********************************************************** */

static mlib_status mlib_v_conv2x2_8nw_2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

static mlib_status mlib_v_conv2x2_8nw_3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

static mlib_status mlib_v_conv2x2_8nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

static mlib_status mlib_v_conv2x2_8nw_mask(
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

#define	LOAD_KERNEL_INTO_FLOAT()                                       \
	k12 =                                                          \
	vis_to_float((kernel[0] & 0xFFFF0000) | ((kernel[1] >> 16) &   \
	    0xFFFF));                                                  \
	k34 =                                                          \
	vis_to_float((kernel[2] & 0xFFFF0000) | ((kernel[3] >> 16) &   \
	    0xFFFF))

/* *********************************************************** */

#define	PREPARE_INTERM_BUFFERS()                                          \
	buff_src =                                                        \
	    (mlib_d64 *)__mlib_malloc(3 * buf_slb * sizeof (mlib_d64));   \
	if (buff_src == NULL)                                             \
	    return (MLIB_FAILURE);                                        \
	sbuf1 = buff_src;                                                 \
	sbuf2 = sbuf1 + buf_slb;                                          \
	dbuf = sbuf2 + buf_slb

/* *********************************************************** */

#define	LOAD_LINE_INTO_BUFFER(p_buf, p_line, n)                         \
	if (!((mlib_s32)p_line & 7)) {                                  \
		for (i = 0; i < (dw + n + 7)/8; i++) {                  \
			p_buf[i] = ((mlib_d64 *)p_line)[i];             \
		}                                                       \
	} else {                                                        \
		mlib_ImageCopy_na((void*)p_line, (void*)p_buf, dw + n); \
	}

/* *********************************************************** */

#define	LOOP_INI()                                              \
	ddst = (mlib_d64 *)dbuf;                                \
	da = d_a;                                               \
	dend = da + (dw - 1);                                   \
	prow = sbuf1;                                           \
	sbuf1 = sbuf2;                                          \
	sbuf2 = prow;                                           \
	s1 = (mlib_d64 *)sbuf1;                                 \
	s2 = (mlib_d64 *)sbuf2

/* *********************************************************** */

#define	PREPARE_TO_COPY_INTERM_BUF_TO_DST()                     \
	ddst = dbuf;                                            \
/*                                                              \
 * prepare the destination addresses                            \
 */                                                             \
	dp = (mlib_d64 *)((mlib_addr)da & (~7));                \
	i = (mlib_addr)dp - (mlib_addr)da;                      \
	cmask1 = cmask >> (-i);                                 \
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
	    vis_pst_8(sd0, dp++, emask & cmask1);               \
	    ddst++;                                             \
	    i += 8;                                             \
	}

/* *********************************************************** */

#define	COPY_INTERM_BUF_TO_DST()                                \
	for (; i <= (dw - 8); i += 8) {                         \
	    sd0 = sd1;                                          \
	    sd1 = ddst[1];                                      \
	    sd00 = vis_faligndata(sd0, sd1);                    \
	    vis_pst_8(sd00, dp++, cmask1);                      \
	    ddst++;                                             \
	}

/* *********************************************************** */

#define	COPY_TAIL()                                             \
	if (i < dw) {                                           \
	    sd0 = vis_faligndata(sd1, ddst[1]);                 \
	    emask = vis_edge8(dp, dend);                        \
	    vis_pst_8(sd0, dp, emask & cmask1);                 \
	}

/* *********************************************************** */

#define	LOAD_INSERT_STORE_U8(channeld)                          \
	sd = (*ddst++);                                         \
	vis_st_u8(sd = vis_faligndata(sd, sd), da);             \
	da += channeld;                                         \
	vis_st_u8(sd = vis_faligndata(sd, sd), da);             \
	da += channeld;                                         \
	vis_st_u8(sd = vis_faligndata(sd, sd), da);             \
	da += channeld;                                         \
	vis_st_u8(sd = vis_faligndata(sd, sd), da);             \
	da += channeld;                                         \
	vis_st_u8(sd = vis_faligndata(sd, sd), da);             \
	da += channeld;                                         \
	vis_st_u8(sd = vis_faligndata(sd, sd), da);             \
	da += channeld;                                         \
	vis_st_u8(sd = vis_faligndata(sd, sd), da);             \
	da += channeld;                                         \
	vis_st_u8(sd = vis_faligndata(sd, sd), da);             \
	da += channeld

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

mlib_status
mlib_v_conv2x2_8nw_2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *prow;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* data */
	mlib_d64 d1, d2, d_1, d_2, d21, d22;
	mlib_f32 k12, k34;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1;
	mlib_d64 tmp0, tmp1, rnd;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1, sd00;
	mlib_s32 emask, cmask1;
	mlib_s32 rval, gsr_scale, i, j;

	gsr_scale = 31 - scalef_expon;
	vis_write_gsr((gsr_scale << 3));
	rval = mlib_round_8[gsr_scale];
	rnd = vis_freg_pair(vis_to_float(rval), vis_to_float(rval));

	cmask &= 3;
	cmask |= (cmask << 2);
	cmask |= (cmask << 4);
	cmask |= (cmask << 8);

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	buf_slb = (2 * dw + 16) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 1;
	dw <<= 1;
	dh -= 1;

	sa = adr_src;
	sa1 = sa + slb;
	d_a = adr_dst;

/* load interm. src buff */
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(sbuf2, sa, 2);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER(sbuf2, sa1, 2);

		vis_alignaddr(s1, 2);
		d1 = *s1;
		d2 = *s2;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k12);
			d21 = vis_faligndata(d1, d_1);
			CONV_AU(d2, k34);
			d22 = vis_faligndata(d2, d_2);
			CONV_AL(d21, k12);
			CONV_AL(d22, k34);
			(*ddst++) = vis_fpack16_pair(out0, out1);
			d1 = d_1;
			d2 = d_2;
			s1++;
			s2++;
		}

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();

		sa1 = sa1 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_conv2x2_8nw_3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *prow;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* data */
	mlib_d64 d1, d2, d_1, d_2, d21, d22;
	mlib_f32 k12, k34;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1;
	mlib_d64 tmp0, tmp1, rnd;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 emask, cmask0, cmask1, cmask2;
	mlib_s32 rval, gsr_scale, i, j;

	gsr_scale = 31 - scalef_expon;
	vis_write_gsr((gsr_scale << 3));
	rval = mlib_round_8[gsr_scale];
	rnd = vis_freg_pair(vis_to_float(rval), vis_to_float(rval));

	cmask &= 7;
	cmask |= (cmask << 3);
	cmask |= (cmask << 6);
	cmask |= (cmask << 12);

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	buf_slb = (3 * dw + 16) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 1;
	dw *= 3;
	dh -= 1;

	sa = adr_src;
	sa1 = sa + slb;
	d_a = adr_dst;

/* load interm. src buff */
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(sbuf2, sa, 3);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER(sbuf2, sa1, 3);

		vis_alignaddr(s1, 3);
		d1 = *s1;
		d2 = *s2;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k12);
			d21 = vis_faligndata(d1, d_1);
			CONV_AU(d2, k34);
			d22 = vis_faligndata(d2, d_2);
			CONV_AL(d21, k12);
			CONV_AL(d22, k34);
			(*ddst++) = vis_fpack16_pair(out0, out1);
			d1 = d_1;
			d2 = d_2;
			s1++;
			s2++;
		}

		ddst = dbuf;
/* prepare the destination addresses */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		i = (mlib_addr)dp - (mlib_addr)da;
		cmask2 = (cmask >> (1 - i));
		cmask0 = cmask2 >> 1;
		cmask1 = cmask0 >> 1;
		ddst = vis_alignaddr(ddst, i);
/* generate edge mask for the start point */
		emask = vis_edge8(da, dend) & cmask2;
		sd0 = (*ddst++);
		sd1 = (*ddst++);
		sd0 = vis_faligndata(sd0, sd1);
		vis_pst_8(sd0, dp++, emask);
		sd0 = sd1;
		i = (mlib_s32)((mlib_u8 *)dp - da);

#pragma pipeloop(0)
		for (; i < (dw - 24); i += 24) {
			sd1 = (*ddst++);
			vis_pst_8(vis_faligndata(sd0, sd1), dp, cmask0);
			sd0 = (*ddst++);
			vis_pst_8(vis_faligndata(sd1, sd0), dp + 1, cmask1);
			sd1 = (*ddst++);
			vis_pst_8(vis_faligndata(sd0, sd1), dp + 2, cmask2);
			sd0 = sd1;
			dp += 3;
		}

		if (i < (dw - 8)) {
			sd1 = (*ddst++);
			vis_pst_8(vis_faligndata(sd0, sd1), dp++, cmask0);
			sd0 = sd1;
			i += 8;

			if (i < (dw - 8)) {
				sd1 = (*ddst++);
				vis_pst_8(vis_faligndata(sd0, sd1), dp++,
				    cmask1);
				sd0 = sd1;
				cmask0 = cmask2;
				i += 8;
			} else {
				cmask0 = cmask1;
			}
		}

		if (i < dw) {
			sd1 = (*ddst++);
			emask = vis_edge8(dp, dend) & cmask0;
			vis_pst_8(vis_faligndata(sd0, sd1), dp, emask);
		}

		sa1 = sa1 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_conv2x2_8nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *prow;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* data */
	mlib_d64 d1, d2, d_1, d_2, d21, d22;
	mlib_f32 k12, k34;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1;
	mlib_d64 tmp0, tmp1, rnd;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1, sd00;
	mlib_s32 emask, cmask1;
	mlib_s32 rval, gsr_scale, i, j;

	gsr_scale = 31 - scalef_expon;
	vis_write_gsr((gsr_scale << 3));
	rval = mlib_round_8[gsr_scale];
	rnd = vis_freg_pair(vis_to_float(rval), vis_to_float(rval));

	cmask = ((cmask & 0xf) << 4) + (cmask & 0xf);
	cmask = (cmask << 8) + (cmask);

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	buf_slb = (4 * dw + 16) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 1;
	dw *= 4;
	dh -= 1;

	sa = adr_src;
	sa1 = sa + slb;
	d_a = adr_dst;

/* load interm. src buff */
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(sbuf2, sa, 4);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER(sbuf2, sa1, 4);

		vis_alignaddr(s1, 4);
		d1 = *s1;
		d2 = *s2;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k12);
			d21 = vis_faligndata(d1, d_1);
			CONV_AU(d2, k34);
			d22 = vis_faligndata(d2, d_2);
			CONV_AL(d21, k12);
			CONV_AL(d22, k34);
			(*ddst++) = vis_fpack16_pair(out0, out1);
			d1 = d_1;
			d2 = d_2;
			s1++;
			s2++;
		}

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();

		sa1 = sa1 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_conv2x2_8nw_mask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1, *sa2, *sa_2;

/* pointers to rows in interm. src buf */
	mlib_u8 *buff_src, *sbuf1, *sbuf2, *s_buf1, *prow;

/* pointer to row in interm. dst buf */
	mlib_u8 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, rnd, sd;

/* data */
	mlib_d64 d1, d2, d_1, d_2;

/* shifted data */
	mlib_d64 d21, d22;

/* coefficients */
	mlib_f32 k12, k34;
	mlib_s32 rval, gsr_scale, i, j, nchannel, chan, testchan;
	mlib_u8 t0, t1, t2, t3, t4, t5, t6, t7;

	gsr_scale = 31 - scalef_expon;
	vis_write_gsr((gsr_scale << 3) + 1);
	rval = mlib_round_8[gsr_scale];
	rnd = vis_freg_pair(vis_to_float(rval), vis_to_float(rval));

	GET_SRC_DST_PARAMETERS();
	nchannel = mlib_ImageGetChannels(src);
	LOAD_KERNEL_INTO_FLOAT();

/* buf_slb - 8-byte aligned */
	buf_slb = (dw + 16) & (~7);
/* alloc. interm. src and dst buffer */
	buff_src = (mlib_u8 *)__mlib_malloc(3 * buf_slb * sizeof (mlib_u8) + 8);

	if (buff_src == NULL)
		return (MLIB_FAILURE);
	sbuf1 = (mlib_u8 *)((mlib_addr)(buff_src + 8) & (~7));
	sbuf2 = sbuf1 + buf_slb;
	dbuf = sbuf2 + buf_slb;

	dw -= 1;
/* edge - no write */
	dh -= 1;

	testchan = 1;

	for (chan = nchannel - 1; chan >= 0; chan--) {
		if ((cmask & testchan) == 0) {
			testchan <<= 1;
			continue;
		}

		testchan <<= 1;
		sa = adr_src + chan;
		sa1 = sa + slb;
		sa_2 = sa2 = sa1 + slb;
		d_a = adr_dst + chan;

/* load interm. src buff */
		for (i = 0, j = 0; j < (dw + 1); i += nchannel, j++) {
			sbuf1[j] = sa1[i];
			sbuf2[j] = sa[i];
		}

		for (j = 0; j < dh - 1; j++) {
			LOOP_INI();
			dend = da + (dw - 1) * nchannel;
			s_buf1 = sbuf1;
			d1 = *s1;
			d2 = *s2;

#pragma pipeloop(0)
/* in each iteration load data for processing next row */
			for (i = 0; i <= (dw - 8); i += 8) {
				t0 = *sa_2;
				sa_2 += nchannel;
				t1 = *sa_2;
				sa_2 += nchannel;
				t2 = *sa_2;
				sa_2 += nchannel;
				t3 = *sa_2;
				sa_2 += nchannel;
				t4 = *sa_2;
				sa_2 += nchannel;
				t5 = *sa_2;
				sa_2 += nchannel;
				t6 = *sa_2;
				sa_2 += nchannel;
				t7 = *sa_2;
				sa_2 += nchannel;
				d_1 = *(s1 + 1);
				d_2 = *(s2 + 1);
				out0 = out1 = rnd;
				CONV_AU(d1, k12);
				(*s_buf1++) = t0;
				(*s_buf1++) = t1;
				d21 = vis_faligndata(d1, d_1);
				CONV_AU(d2, k34);
				(*s_buf1++) = t2;
				(*s_buf1++) = t3;
				d22 = vis_faligndata(d2, d_2);
				CONV_AL(d21, k12);
				(*s_buf1++) = t4;
				(*s_buf1++) = t5;
				CONV_AL(d22, k34);
				(*s_buf1++) = t6;
				(*s_buf1++) = t7;
				(*ddst++) = vis_fpack16_pair(out0, out1);
				d1 = d_1;
				d2 = d_2;
				s1++;
				s2++;
			}
			if (dw & 7) {
				d_1 = *(s1 + 1);
				d_2 = *(s2 + 1);
				out0 = out1 = rnd;
				CONV_AU(d1, k12);
				d21 = vis_faligndata(d1, d_1);
				CONV_AU(d2, k34);
				d22 = vis_faligndata(d2, d_2);
				CONV_AL(d21, k12);
				CONV_AL(d22, k34);
				(*ddst++) = vis_fpack16_pair(out0, out1);
				d1 = d_1;
				d2 = d_2;
				s1++;
				s2++;
			}

			for (; i < dw + 1; i++) {
				(*s_buf1++) = *sa_2;
				sa_2 += nchannel;
			}

			ddst = (mlib_d64 *)(dbuf);
#pragma pipeloop(0)
			for (i = 0; i <= (dw - 8); i += 8) {
				LOAD_INSERT_STORE_U8(nchannel);
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				sd = (*ddst++);
				vis_st_u8(sd = vis_faligndata(sd, sd), da);
				da += nchannel;

				if ((mlib_addr)da <= (mlib_addr)dend) {
					vis_st_u8(sd =
					    vis_faligndata(sd, sd), da);
					da += nchannel;

					if ((mlib_addr)da <= (mlib_addr)dend) {
						vis_st_u8(sd =
						    vis_faligndata(sd, sd), da);
						da += nchannel;

						if ((mlib_addr)da <=
						    (mlib_addr)dend) {
						    vis_st_u8(sd =
							vis_faligndata(sd,
							sd), da);
						    da += nchannel;

						    if ((mlib_addr)da <=
							(mlib_addr)dend) {
							vis_st_u8(sd =
							    vis_faligndata
							    (sd, sd), da);
							da += nchannel;

							if ((mlib_addr) da <=
							    (mlib_addr) dend) {
							    vis_st_u8(sd =
								vis_faligndata
								(sd, sd), da);
							    da += nchannel;

							    if ((mlib_addr)da <=
								(mlib_addr)
								dend) {
								vis_st_u8(sd =
								vis_faligndata
								    (sd, sd),
								    da);
							    }
							}
						    }
						}
					}
				}
			}

			sa_2 = sa2 = sa2 + slb;
			d_a += dlb;
		}

/* process last row - no need to load data */
		LOOP_INI();
		dend = da + (dw - 1) * nchannel;
		d1 = *s1;
		d2 = *s2;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k12);
			d21 = vis_faligndata(d1, d_1);
			CONV_AU(d2, k34);
			d22 = vis_faligndata(d2, d_2);
			CONV_AL(d21, k12);
			CONV_AL(d22, k34);
			(*ddst++) = vis_fpack16_pair(out0, out1);
			d1 = d_1;
			d2 = d_2;
			s1++;
			s2++;
		}

		ddst = (mlib_d64 *)(dbuf);
#pragma pipeloop(0)
		for (i = 0; i <= (dw - 8); i += 8) {
			LOAD_INSERT_STORE_U8(nchannel);
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			sd = (*ddst++);
			vis_st_u8(sd = vis_faligndata(sd, sd), da);
			da += nchannel;

			if ((mlib_addr)da <= (mlib_addr)dend) {
				vis_st_u8(sd = vis_faligndata(sd, sd), da);
				da += nchannel;

				if ((mlib_addr)da <= (mlib_addr)dend) {
					vis_st_u8(sd =
					    vis_faligndata(sd, sd), da);
					da += nchannel;

					if ((mlib_addr)da <= (mlib_addr)dend) {
						vis_st_u8(sd =
						    vis_faligndata(sd, sd), da);
						da += nchannel;

						if ((mlib_addr)da <=
						    (mlib_addr)dend) {
						    vis_st_u8(sd =
							vis_faligndata(sd, sd),
							da);
						    da += nchannel;

						    if ((mlib_addr)da <=
							(mlib_addr)dend) {
							vis_st_u8(sd =
							    vis_faligndata
							    (sd, sd), da);
							da += nchannel;

							if ((mlib_addr) da <=
							    (mlib_addr) dend) {
							    vis_st_u8(sd =
								vis_faligndata
								(sd, sd), da);
							}
						    }
						}
					}
				}
			}
		}
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_conv2x2nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchannel, chan, cmask1, i;
	mlib_status res;

	if (mlib_ImageConvVersion(2, 2, scalef_expon, MLIB_BYTE) == 0)
		return mlib_c_conv2x2nw_u8(dst, src, kernel, scalef_expon,
		    cmask);

	cmask1 = cmask;
	nchannel = mlib_ImageGetChannels(src);
	chan = 0;
	for (i = nchannel - 1; i >= 0; i--, cmask1 >>= 1)
		if ((cmask1 & 1) == 1)
			chan++;

	if (nchannel == chan) {
		return (mlib_conv2x2_8nw_f(dst, src, kernel, scalef_expon));
	}

	if ((chan == 1) && ((nchannel == 3) || (nchannel == 4))) {
		res =
		    mlib_v_conv2x2_8nw_mask(dst, src, kernel, scalef_expon,
		    cmask);
	} else {
		switch (nchannel) {
		case 2:
			res =
			    mlib_v_conv2x2_8nw_2(dst, src, kernel, scalef_expon,
			    cmask);
			break;
		case 3:
			res =
			    mlib_v_conv2x2_8nw_3(dst, src, kernel, scalef_expon,
			    cmask);
			break;
		case 4:
			res =
			    mlib_v_conv2x2_8nw_4(dst, src, kernel, scalef_expon,
			    cmask);
			break;
		default:
			res = MLIB_FAILURE;
			break;
		}
	}

	return (res);
}

/* *********************************************************** */
