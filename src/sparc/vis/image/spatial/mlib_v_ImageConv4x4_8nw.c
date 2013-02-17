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

#pragma ident	"@(#)mlib_v_ImageConv4x4_8nw.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_conv4x4_8nw.c - convolve an 8-bit image with a 4x4 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *      mlib_status mlib_conv4x4nw_u8(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *kernel,
 *                                    mlib_s32         scalef_expon,
 *                                    mlib_s32         cmask)
 *
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
 *      A 2-D convolution (4x4 kernel) for 8-bit images.
 */

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
#include <mlib_v_ImageConv.h>

/* *********************************************************** */

static mlib_status mlib_v_conv4x4_8nw_1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_conv4x4_8nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

static mlib_status mlib_v_conv4x4_8nw_mask(
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
	    0xFFFF))

/* *********************************************************** */

#define	PREPARE_INTERM_BUFFERS()                                          \
	buff_src =                                                        \
	    (mlib_d64 *)__mlib_malloc(5 * buf_slb * sizeof (mlib_d64));   \
	if (buff_src == NULL)                                             \
	    return (MLIB_FAILURE);                                        \
	sbuf1 = buff_src;                                                 \
	sbuf2 = sbuf1 + buf_slb;                                          \
	sbuf3 = sbuf2 + buf_slb;                                          \
	sbuf4 = sbuf3 + buf_slb;                                          \
	dbuf = sbuf4 + buf_slb

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
	sbuf4 = prow;                                           \
	s1 = (mlib_d64 *)sbuf1;                                 \
	s2 = (mlib_d64 *)sbuf2;                                 \
	s3 = (mlib_d64 *)sbuf3;                                 \
	s4 = (mlib_d64 *)sbuf4

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

#define	SHIFT_U8                                                \
	d21 = vis_faligndata(d21, dt_1);                        \
	dt_1 = vis_faligndata(dt_1, d1);                        \
	d22 = vis_faligndata(d22, dt_2);                        \
	dt_2 = vis_faligndata(dt_2, d2);                        \
	d23 = vis_faligndata(d23, dt_3);                        \
	dt_3 = vis_faligndata(dt_3, d3);                        \
	d24 = vis_faligndata(d24, dt_4);                        \
	dt_4 = vis_faligndata(dt_4, d4)

/* *********************************************************** */

mlib_status
mlib_v_conv4x4_8nw_1(
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
	mlib_u8 *sa, *sa1, *sa2, *sa3;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf3, *sbuf4;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* data */
	mlib_d64 d1, d2, d_1, d_2, d21, d22;

/* data */
	mlib_d64 d3, d4, d_3, d_4, d23, d24;

/* data */
	mlib_d64 dt_1, dt_2, dt_3, dt_4;
	mlib_f32 k1k2, k3k4, k5k6, k7k8;
	mlib_f32 k9k10, k11k12, k13k14, k15k16;

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

	buf_slb = (dw + 16) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 3;
	dh -= 3;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	d_a = adr_dst + dlb + 1;

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(3);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(3);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(3);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf4, sa3);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(3);

		vis_alignaddr(s1, 1);
		d1 = *s1;
		d2 = *s2;
		d3 = *s3;
		d4 = *s4;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_3 = *(s3 + 1);
			d_4 = *(s4 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k1k2);
			CONV_AU(d2, k5k6);
			CONV_AU(d3, k9k10);
			CONV_AU(d4, k13k14);
			d21 = vis_faligndata(d1, d_1);
			dt_1 = vis_faligndata(d_1, d1);
			d22 = vis_faligndata(d2, d_2);
			dt_2 = vis_faligndata(d_2, d2);
			d23 = vis_faligndata(d3, d_3);
			dt_3 = vis_faligndata(d_3, d3);
			d24 = vis_faligndata(d4, d_4);
			dt_4 = vis_faligndata(d_4, d4);
			CONV_AL(d21, k1k2);
			CONV_AL(d22, k5k6);
			CONV_AL(d23, k9k10);
			CONV_AL(d24, k13k14);
			SHIFT_U8;
			CONV_AU(d21, k3k4);
			CONV_AU(d22, k7k8);
			CONV_AU(d23, k11k12);
			CONV_AU(d24, k15k16);
			d21 = vis_faligndata(d21, dt_1);
			d22 = vis_faligndata(d22, dt_2);
			d23 = vis_faligndata(d23, dt_3);
			d24 = vis_faligndata(d24, dt_4);
			CONV_AL(d21, k3k4);
			CONV_AL(d22, k7k8);
			CONV_AL(d23, k11k12);
			CONV_AL(d24, k15k16);
			(*ddst++) = vis_fpack16_pair(out0, out1);
			d1 = d_1;
			d2 = d_2;
			d3 = d_3;
			d4 = d_4;
			s1++;
			s2++;
			s3++;
			s4++;
		}

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();

		sa3 = sa3 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_conv4x4_8nw_4(
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
	mlib_u8 *sa, *sa1, *sa2, *sa3;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf3, *sbuf4;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* data */
	mlib_d64 d1, d2, d_1, d_2, d21, d22;

/* data */
	mlib_d64 d3, d4, d_3, d_4, d23, d24;

/* data */
	mlib_d64 d_21, d_22, d_23, d_24;
	mlib_f32 k1k2, k3k4, k5k6, k7k8;
	mlib_f32 k9k10, k11k12, k13k14, k15k16;

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

	buf_slb = (4 * dw + 24) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 3;
	dw *= 4;
	dh -= 3;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	d_a = adr_dst + dlb + 4;

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(12);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(12);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(12);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf4, sa3);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(12);

		vis_alignaddr(s1, 4);
		d1 = *s1;
		d2 = *s2;
		d3 = *s3;
		d4 = *s4;
		s1++;
		s2++;
		s3++;
		s4++;
		d_1 = *s1;
		d_2 = *s2;
		d_3 = *s3;
		d_4 = *s4;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_21 = *(s1 + 1);
			d_22 = *(s2 + 1);
			d_23 = *(s3 + 1);
			d_24 = *(s4 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k1k2);
			CONV_AU(d2, k5k6);
			CONV_AU(d3, k9k10);
			CONV_AU(d4, k13k14);
			d21 = vis_faligndata(d1, d_1);
			d22 = vis_faligndata(d2, d_2);
			d23 = vis_faligndata(d3, d_3);
			d24 = vis_faligndata(d4, d_4);
			CONV_AL(d21, k1k2);
			CONV_AL(d22, k5k6);
			CONV_AL(d23, k9k10);
			CONV_AL(d24, k13k14);
			CONV_AU(d_1, k3k4);
			CONV_AU(d_2, k7k8);
			CONV_AU(d_3, k11k12);
			CONV_AU(d_4, k15k16);
			d21 = vis_faligndata(d_1, d_21);
			d22 = vis_faligndata(d_2, d_22);
			d23 = vis_faligndata(d_3, d_23);
			d24 = vis_faligndata(d_4, d_24);
			CONV_AL(d21, k3k4);
			CONV_AL(d22, k7k8);
			CONV_AL(d23, k11k12);
			CONV_AL(d24, k15k16);
			(*ddst++) = vis_fpack16_pair(out0, out1);
			d1 = d_1;
			d2 = d_2;
			d3 = d_3;
			d4 = d_4;
			d_1 = d_21;
			d_2 = d_22;
			d_3 = d_23;
			d_4 = d_24;
			s1++;
			s2++;
			s3++;
			s4++;
		}

		ddst = dbuf;
/* prepare the destination addresses */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		i = (mlib_addr)dp - (mlib_addr)da;
		cmask1 = cmask >> (-i);
		ddst = vis_alignaddr(ddst, i);
/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);
		sd1 = ddst[0];

		if (emask != 0xff) {
			sd0 = sd1;
			sd1 = ddst[1];
			sd0 = vis_faligndata(sd0, sd1);
			vis_pst_8(sd0, dp++, emask & cmask1);
			ddst++;
			i += 8;
		}
#pragma pipeloop(0)
		for (; i <= (dw - 8); i += 8) {
			sd0 = sd1;
			sd1 = ddst[1];
			sd00 = vis_faligndata(sd0, sd1);
			vis_pst_8(sd00, dp++, cmask1);
			ddst++;
		}

		if (i < dw) {
			sd0 = vis_faligndata(sd1, ddst[1]);
			emask = vis_edge8(dp, dend);
			vis_pst_8(sd0, dp, emask & cmask1);
		}

		sa3 = sa3 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_conv4x4_8nw_mask(
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
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4, *sa_4;

/* pointers to rows in interm. src buf */
	mlib_u8 *buff_src, *sbuf1, *sbuf2, *s_buf1, *prow;

/* pointers to rows in interm. src buf */
	mlib_u8 *sbuf3, *sbuf4;

/* pointer to row in interm. dst buf */
	mlib_u8 *dbuf, *d_buf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, rnd;

/* data */
	mlib_d64 d1, d2, d_1, d_2, d21, d22;

/* data */
	mlib_d64 d3, d4, d_3, d_4, d23, d24;

/* data */
	mlib_d64 dt_1, dt_2, dt_3, dt_4;
	mlib_f32 k1k2, k3k4, k5k6, k7k8;
	mlib_f32 k9k10, k11k12, k13k14, k15k16;
	mlib_s32 rval, gsr_scale, i, j, nchannel, nchannel1, chan, testchan;
	mlib_u8 t1, t2, t3, t4, t5, t6, t7, t8;
	mlib_u8 tt1, tt2, tt3, tt4, tt5, tt6, tt7, tt8;

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
	buff_src = (mlib_u8 *)__mlib_malloc(5 * buf_slb * sizeof (mlib_u8) + 8);

	if (buff_src == NULL)
		return (MLIB_FAILURE);
	sbuf1 = (mlib_u8 *)((mlib_addr)(buff_src + 8) & (~7));
	sbuf2 = sbuf1 + buf_slb;
	sbuf3 = sbuf2 + buf_slb;
	sbuf4 = sbuf3 + buf_slb;
	dbuf = sbuf4 + buf_slb;

	dw -= 3;
/* edge - no write */
	dh -= 3;

	testchan = 1;

	for (chan = nchannel - 1; chan >= 0; chan--) {
		if ((cmask & testchan) == 0) {
			testchan <<= 1;
			continue;
		}

		testchan <<= 1;
		sa = adr_src + chan;
		sa1 = sa + slb;
		sa2 = sa1 + slb;
		sa3 = sa2 + slb;
		sa_4 = sa4 = sa3 + slb;
		d_a = adr_dst + dlb + nchannel + chan;

/* load interm. src buff */
		for (i = 0, j = 0; j < (dw + 3); i += nchannel, j++) {
			sbuf1[j] = sa3[i];
			sbuf2[j] = sa[i];
			sbuf3[j] = sa1[i];
			sbuf4[j] = sa2[i];
		}

		for (j = 0; j < dh - 1; j++) {
			LOOP_INI();
			dend = da + (dw - 1) * nchannel;
			s_buf1 = sbuf1;
			d_buf = (dbuf - 8);
			nchannel1 = 0;
			d1 = *s1;
			d2 = *s2;
			d3 = *s3;
			d4 = *s4;

/* in each iteration load data for processing next row */
#pragma pipeloop(0)
			for (i = 0; i < dw; i += 8) {
				d_1 = *(s1 + 1);
				d_2 = *(s2 + 1);
				d_3 = *(s3 + 1);
				d_4 = *(s4 + 1);
				out0 = out1 = rnd;
				t1 = vis_ld_u8_nf(sa_4);
				sa_4 += nchannel;
				CONV_AU(d1, k1k2);
				t2 = vis_ld_u8_nf(sa_4);
				sa_4 += nchannel;
				CONV_AU(d2, k5k6);
				t3 = vis_ld_u8_nf(sa_4);
				sa_4 += nchannel;
				CONV_AU(d3, k9k10);
				t4 = vis_ld_u8_nf(sa_4);
				sa_4 += nchannel;
				CONV_AU(d4, k13k14);
				t5 = vis_ld_u8_nf(sa_4);
				sa_4 += nchannel;
				d21 = vis_faligndata(d1, d_1);
				t6 = vis_ld_u8_nf(sa_4);
				sa_4 += nchannel;
				dt_1 = vis_faligndata(d_1, d1);
				t7 = vis_ld_u8_nf(sa_4);
				sa_4 += nchannel;
				d22 = vis_faligndata(d2, d_2);
				t8 = vis_ld_u8_nf(sa_4);
				sa_4 += nchannel;
				dt_2 = vis_faligndata(d_2, d2);
				tt1 = (*d_buf++);
				d23 = vis_faligndata(d3, d_3);
				tt2 = (*d_buf++);
				dt_3 = vis_faligndata(d_3, d3);
				tt3 = (*d_buf++);
				d24 = vis_faligndata(d4, d_4);
				tt4 = (*d_buf++);
				dt_4 = vis_faligndata(d_4, d4);
				tt5 = (*d_buf++);
				CONV_AL(d21, k1k2);
				tt6 = (*d_buf++);
				CONV_AL(d22, k5k6);
				tt7 = (*d_buf++);
				CONV_AL(d23, k9k10);
				tt8 = (*d_buf++);
				CONV_AL(d24, k13k14);
				(*s_buf1++) = t1;
				SHIFT_U8;
				(*s_buf1++) = t2;
				CONV_AU(d21, k3k4);
				(*s_buf1++) = t3;
				CONV_AU(d22, k7k8);
				(*s_buf1++) = t4;
				CONV_AU(d23, k11k12);
				(*s_buf1++) = t5;
				CONV_AU(d24, k15k16);
				(*s_buf1++) = t6;
				d21 = vis_faligndata(d21, dt_1);
				(*s_buf1++) = t7;
				d22 = vis_faligndata(d22, dt_2);
				(*s_buf1++) = t8;
				d23 = vis_faligndata(d23, dt_3);
				*da = tt1;
				da += nchannel1;
				d24 = vis_faligndata(d24, dt_4);
				*da = tt2;
				da += nchannel1;
				CONV_AL(d21, k3k4);
				*da = tt3;
				da += nchannel1;
				CONV_AL(d22, k7k8);
				*da = tt4;
				da += nchannel1;
				CONV_AL(d23, k11k12);
				*da = tt5;
				da += nchannel1;
				CONV_AL(d24, k15k16);
				*da = tt6;
				da += nchannel1;
				(*ddst++) = vis_fpack16_pair(out0, out1);
				*da = tt7;
				da += nchannel1;
				d1 = d_1;
				d2 = d_2;
				d3 = d_3;
				d4 = d_4;
				*da = tt8;
				da += nchannel1;
				s1++;
				s2++;
				s3++;
				s4++;
				nchannel1 = nchannel;
			}

			(*s_buf1++) = vis_ld_u8_nf(sa_4);
			sa_4 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_4);
			sa_4 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_4);
			sa_4 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_4);
			sa_4 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_4);
			sa_4 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_4);
			sa_4 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_4);
			sa_4 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_4);

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

			sa_4 = sa4 = sa4 + slb;
			d_a += dlb;
		}

/* process last row - no need to load data */
		LOOP_INI();
		dend = da + (dw - 1) * nchannel;
		d_buf = (dbuf - 8);
		nchannel1 = 0;
		d1 = *s1;
		d2 = *s2;
		d3 = *s3;
		d4 = *s4;

/* in each iteration load data for processing next row */
#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_3 = *(s3 + 1);
			d_4 = *(s4 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k1k2);
			CONV_AU(d2, k5k6);
			CONV_AU(d3, k9k10);
			CONV_AU(d4, k13k14);
			d21 = vis_faligndata(d1, d_1);
			dt_1 = vis_faligndata(d_1, d1);
			d22 = vis_faligndata(d2, d_2);
			dt_2 = vis_faligndata(d_2, d2);
			tt1 = (*d_buf++);
			d23 = vis_faligndata(d3, d_3);
			tt2 = (*d_buf++);
			dt_3 = vis_faligndata(d_3, d3);
			tt3 = (*d_buf++);
			d24 = vis_faligndata(d4, d_4);
			tt4 = (*d_buf++);
			dt_4 = vis_faligndata(d_4, d4);
			tt5 = (*d_buf++);
			CONV_AL(d21, k1k2);
			tt6 = (*d_buf++);
			CONV_AL(d22, k5k6);
			tt7 = (*d_buf++);
			CONV_AL(d23, k9k10);
			tt8 = (*d_buf++);
			CONV_AL(d24, k13k14);
			SHIFT_U8;
			*da = tt1;
			da += nchannel1;
			CONV_AU(d21, k3k4);
			*da = tt2;
			da += nchannel1;
			CONV_AU(d22, k7k8);
			*da = tt3;
			da += nchannel1;
			CONV_AU(d23, k11k12);
			*da = tt4;
			da += nchannel1;
			CONV_AU(d24, k15k16);
			*da = tt5;
			da += nchannel1;
			d21 = vis_faligndata(d21, dt_1);
			*da = tt6;
			da += nchannel1;
			d22 = vis_faligndata(d22, dt_2);
			*da = tt7;
			da += nchannel1;
			d23 = vis_faligndata(d23, dt_3);
			*da = tt8;
			da += nchannel1;
			d24 = vis_faligndata(d24, dt_4);
			CONV_AL(d21, k3k4);
			CONV_AL(d22, k7k8);
			CONV_AL(d23, k11k12);
			CONV_AL(d24, k15k16);
			(*ddst++) = vis_fpack16_pair(out0, out1);
			d1 = d_1;
			d2 = d_2;
			d3 = d_3;
			d4 = d_4;
			s1++;
			s2++;
			s3++;
			s4++;
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
mlib_conv4x4nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchan, amask;

	if (mlib_ImageConvVersion(4, 4, scalef_expon, MLIB_BYTE) == 0)
		return mlib_c_conv4x4nw_u8(dst, src, kernel, scalef_expon,
		    cmask);

	nchan = mlib_ImageGetChannels(src);
	amask = (1 << nchan) - 1;
	cmask &= amask;

	if (nchan == 1) {
		return (mlib_v_conv4x4_8nw_1(dst, src, kernel, scalef_expon));
	}

	if (cmask == amask) {
		return mlib_convMxN_8nw_f(dst, src, 4, 4, 1, 1, kernel,
		    scalef_expon);
	}

	if ((nchan == 4) && (cmask == 0xE || cmask == 0x7)) {
		return mlib_v_conv4x4_8nw_4(dst, src, kernel, scalef_expon,
		    cmask);
	}

	return (mlib_v_conv4x4_8nw_mask(dst, src, kernel, scalef_expon, cmask));
}

/* *********************************************************** */
