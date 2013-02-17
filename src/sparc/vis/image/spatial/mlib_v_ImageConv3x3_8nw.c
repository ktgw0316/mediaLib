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

#pragma ident	"@(#)mlib_v_ImageConv3x3_8nw.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_conv3x3_8nw.c - convolve an 8-bit image with a 3x3 kernel
 *                      (edge = no write)
 *
 *
 * SYNOPSIS
 *  mlib_status mlib_conv3x3nw_u8(mlib_image       *dst,
 *                                const mlib_image *src,
 *                                const mlib_s32   *kernel,
 *                                mlib_s32         scalef_expon,
 *                                mlib_s32         cmask)
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
 *      A 2-D convolution (3x3 kernel) for 8-bit images.
 */

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
#include <mlib_v_ImageConv.h>

/* *********************************************************** */

static mlib_status mlib_v_conv3x3_8nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

static mlib_status mlib_v_conv3x3_8nw_mask(
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

#define	GET_SRC_DST_PARAMETERS()                                \
	adr_src = mlib_ImageGetData(src);                       \
	adr_dst = mlib_ImageGetData(dst);                       \
	slb = mlib_ImageGetStride(src);                         \
	dlb = mlib_ImageGetStride(dst);                         \
	dh = mlib_ImageGetHeight(dst);                          \
	dw = mlib_ImageGetWidth(dst)

/* *********************************************************** */

#define	LOAD_KERNEL_INTO_FLOAT()                                       \
	k1k2 =                                                         \
	vis_to_float((kernel[0] & 0xFFFF0000) | ((kernel[1] >> 16) &   \
	    0xFFFF));                                                  \
	k3k4 =                                                         \
	vis_to_float((kernel[2] & 0xFFFF0000) | ((kernel[3] >> 16) &   \
	    0xFFFF));                                                  \
	k5k6 =                                                         \
	vis_to_float((kernel[4] & 0xFFFF0000) | ((kernel[5] >> 16) &   \
	    0xFFFF));                                                  \
	k7k8 =                                                         \
	vis_to_float((kernel[6] & 0xFFFF0000) | ((kernel[7] >> 16) &   \
	    0xFFFF));                                                  \
	k9k9 =                                                         \
	vis_to_float((kernel[8] & 0xFFFF0000) | ((kernel[8] >> 16) &   \
	    0xFFFF))

/* *********************************************************** */

#define	PREPARE_INTERM_BUFFERS()                                          \
	buff_src =                                                        \
	    (mlib_d64 *)__mlib_malloc(4 * buf_slb * sizeof (mlib_d64));   \
	if (buff_src == NULL)                                             \
	    return (MLIB_FAILURE);                                        \
	sbuf1 = buff_src;                                                 \
	sbuf2 = sbuf1 + buf_slb;                                          \
	sbuf3 = sbuf2 + buf_slb;                                          \
	dbuf = sbuf3 + buf_slb

/* *********************************************************** */

#define	LOAD_LINE_AL(prow, p_line, n)                           \
	dsa = (mlib_d64 *)p_line;                               \
	for (i = 0; i < (dw + n + 7)/8; i++) {                  \
	    prow[i] = dsa[i];                                   \
	}

/* *********************************************************** */

#define	LOAD_LINE_NA(prow, p_line, n)                           \
	mlib_ImageCopy_na(p_line, (void*)prow, dw + n)

/* *********************************************************** */

#define	LOOP_INI()                                              \
	ddst = (mlib_d64 *)dbuf;                                \
	da = d_a;                                               \
	dend = da + (dw - 1);                                   \
	prow = sbuf1;                                           \
	sbuf1 = sbuf2;                                          \
	sbuf2 = sbuf3;                                          \
	sbuf3 = prow;                                           \
	s1 = (mlib_d64 *)sbuf1;                                 \
	s2 = (mlib_d64 *)sbuf2;                                 \
	s3 = (mlib_d64 *)sbuf3

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
mlib_v_conv3x3_8nw_4(
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
	mlib_u8 *sa, *sa1, *sa2;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf3;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* data */
	mlib_d64 d1, d2, d_1, d_2, d21, d22;

/* data */
	mlib_d64 d3, d_3, d23;
	mlib_f32 k1k2, k3k4, k5k6, k7k8, k9k9;

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

	dw -= 2;
	dw *= 4;
	dh -= 2;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	d_a = adr_dst + dlb + 4;

/* load interm. src buff */
	if ((int)sa & 7) {
#pragma pipeloop(0)
		LOAD_LINE_NA(sbuf2, sa, 8);
	} else {
#pragma pipeloop(0)
		LOAD_LINE_AL(sbuf2, sa, 8);
	}

	if ((int)sa1 & 7) {
#pragma pipeloop(0)
		LOAD_LINE_NA(sbuf3, sa1, 8);
	} else {
#pragma pipeloop(0)
		LOAD_LINE_AL(sbuf3, sa1, 8);
	}

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		if ((int)sa2 & 7) {
#pragma pipeloop(0)
			LOAD_LINE_NA(sbuf3, sa2, 8);
		} else {
#pragma pipeloop(0)
			LOAD_LINE_AL(sbuf3, sa2, 8);
		}

		vis_alignaddr(s1, 4);
		d1 = *s1;
		d2 = *s2;
		d3 = *s3;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_3 = *(s3 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k1k2);
			CONV_AL(d2, k3k4);
			CONV_AU(d3, k7k8);
			d21 = vis_faligndata(d1, d_1);
			d22 = vis_faligndata(d2, d_2);
			d23 = vis_faligndata(d3, d_3);
			CONV_AL(d21, k1k2);
			CONV_AU(d22, k5k6);
			CONV_AL(d23, k7k8);
			CONV_AU(d_1, k3k4);
			CONV_AL(d_2, k5k6);
			CONV_AU(d_3, k9k9);
			(*ddst++) = vis_fpack16_pair(out0, out1);
			d1 = d_1;
			d2 = d_2;
			d3 = d_3;
			s1++;
			s2++;
			s3++;
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

		sa2 = sa2 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_conv3x3_8nw_mask(
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
	mlib_u8 *sa, *sa2, *sa3, *sa4, *sa_4;

/* pointers to rows in interm. src buf */
	mlib_u8 *buff_src, *sbuf1, *sbuf2, *sbuf3, *s_buf1, *prow;

/* pointer to row in interm. dst buf */
	mlib_u8 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, rnd, sd;

/* data */
	mlib_d64 d1, d2, d3, d_1, d_2, d_3;

/* temp. data, used in faligndata */
	mlib_d64 dt_1, dt_2, dt_3;

/* shifted data */
	mlib_d64 d21, d22, d23, d31, d32, d33;

/* coefficients */
	mlib_f32 k12, k34, k56, k78, k9;
	mlib_s32 rval, gsr_scale, i, j, nchannel, chan, testchan;

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
	buff_src = (mlib_u8 *)__mlib_malloc(4 * buf_slb * sizeof (mlib_u8) + 8);

	if (buff_src == NULL)
		return (MLIB_FAILURE);
	dw -= 2;
/* edge - no write */
	dh -= 2;

/*
 * *  The 8x16 mult has built-in 8-bit R shift, and fpack16 has 7-bit
 * *  fixed R shift (preceded by variable-bit L shift controlled by GSR
 * *  scalefactor field). Thus net R shift = (8+7)-(GSR.scalefactor_field),
 * *  so GSR.scalefactor_field = 15-(net R shift):
 */
	gsr_scale = 31 - scalef_expon;
	vis_write_gsr((gsr_scale << 3) + 1);
	rval = mlib_round_8[gsr_scale];
	rnd = vis_freg_pair(vis_to_float(rval), vis_to_float(rval));
	sbuf1 = (mlib_u8 *)((mlib_addr)(buff_src + 8) & (~7));
	sbuf2 = sbuf1 + buf_slb;
	sbuf3 = sbuf2 + buf_slb;
	dbuf = sbuf3 + buf_slb;

/* load kernel values into floats */

	k12 =
	    vis_to_float((kernel[0] & 0xFFFF0000) | ((kernel[1] >> 16) &
	    0xFFFF));
	k34 =
	    vis_to_float((kernel[2] & 0xFFFF0000) | ((kernel[3] >> 16) &
	    0xFFFF));
	k56 =
	    vis_to_float((kernel[4] & 0xFFFF0000) | ((kernel[5] >> 16) &
	    0xFFFF));
	k78 =
	    vis_to_float((kernel[6] & 0xFFFF0000) | ((kernel[7] >> 16) &
	    0xFFFF));
	k9 = vis_to_float((kernel[8] & 0xFFFF0000) | ((kernel[8] >> 16) &
	    0xFFFF));

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
		sa_4 = sa4 = sa3 + slb;
		d_a = adr_dst + dlb + nchannel + chan;

/* load interm. src buff */
		for (i = 0, j = 0; j < (dw + 2); i += nchannel, j++) {
			sbuf1[j] = sa3[i];
			sbuf2[j] = sa[i];
			sbuf3[j] = sa2[i];
		}

		for (j = 0; j < dh - 1; j++) {
			ddst = (mlib_d64 *)(dbuf);
			da = d_a;
			dend = da + (dw - 1) * nchannel;
			prow = sbuf1;
			sbuf1 = sbuf2;
			sbuf2 = sbuf3;
			sbuf3 = prow;
			s1 = (mlib_d64 *)sbuf1;
			s2 = (mlib_d64 *)sbuf2;
			s3 = (mlib_d64 *)sbuf3;
			s_buf1 = sbuf1;
			d1 = *s1;
			d2 = *s2;
			d3 = *s3;

#pragma pipeloop(0)
/* in each iteration load data for processing next row */
			for (i = 0; i <= dw - 8; i += 8) {
				d_1 = *(s1 + 1);
				d_2 = *(s2 + 1);
				d_3 = *(s3 + 1);
				out0 = out1 = rnd;
				CONV_AU(d1, k12);
				(*s_buf1++) = *sa_4;
				sa_4 += nchannel;
				CONV_AL(d2, k34);
				(*s_buf1++) = *sa_4;
				sa_4 += nchannel;
				CONV_AU(d3, k78);
				(*s_buf1++) = *sa_4;
				sa_4 += nchannel;
				d22 = vis_faligndata(d2, d_2);
				dt_2 = vis_faligndata(d_2, d2);
				d21 = vis_faligndata(d1, d_1);
				dt_1 = vis_faligndata(d_1, d1);
				d23 = vis_faligndata(d3, d_3);
				dt_3 = vis_faligndata(d_3, d3);
				CONV_AL(d21, k12);
				(*s_buf1++) = *sa_4;
				sa_4 += nchannel;
				CONV_AU(d22, k56);
				(*s_buf1++) = *sa_4;
				sa_4 += nchannel;
				CONV_AL(d23, k78);
				(*s_buf1++) = *sa_4;
				sa_4 += nchannel;
				d31 = vis_faligndata(d21, dt_1);
				d32 = vis_faligndata(d22, dt_2);
				d33 = vis_faligndata(d23, dt_3);
				CONV_AU(d31, k34);
				(*s_buf1++) = *sa_4;
				sa_4 += nchannel;
				CONV_AL(d32, k56);
				(*s_buf1++) = *sa_4;
				sa_4 += nchannel;
				CONV_AU(d33, k9);
				(*ddst++) =
				    vis_freg_pair(vis_fpack16(out0),
				    vis_fpack16(out1));
				d1 = d_1;
				d2 = d_2;
				d3 = d_3;
				s1++;
				s2++;
				s3++;
			}
			if (dw & 7) {
				d_1 = *(s1 + 1);
				d_2 = *(s2 + 1);
				d_3 = *(s3 + 1);
				out0 = out1 = rnd;
				CONV_AU(d1, k12);
				CONV_AL(d2, k34);
				CONV_AU(d3, k78);
				d22 = vis_faligndata(d2, d_2);
				dt_2 = vis_faligndata(d_2, d2);
				d21 = vis_faligndata(d1, d_1);
				dt_1 = vis_faligndata(d_1, d1);
				d23 = vis_faligndata(d3, d_3);
				dt_3 = vis_faligndata(d_3, d3);
				CONV_AL(d21, k12);
				CONV_AU(d22, k56);
				CONV_AL(d23, k78);
				d31 = vis_faligndata(d21, dt_1);
				d32 = vis_faligndata(d22, dt_2);
				d33 = vis_faligndata(d23, dt_3);
				CONV_AU(d31, k34);
				CONV_AL(d32, k56);
				CONV_AU(d33, k9);
				(*ddst++) =
				    vis_freg_pair(vis_fpack16(out0),
				    vis_fpack16(out1));
				d1 = d_1;
				d2 = d_2;
				d3 = d_3;
				s1++;
				s2++;
				s3++;
			}

			for (; i < dw + 2; i++) {
				(*s_buf1++) = *sa_4;
				sa_4 += nchannel;
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

			sa_4 = sa4 = sa4 + slb;
			d_a += dlb;
		}

/* process last row - no need to load data */
		ddst = (mlib_d64 *)(dbuf);
		da = d_a;
		dend = da + (dw - 1) * nchannel;
		prow = sbuf1;
		sbuf1 = sbuf2;
		sbuf2 = sbuf3;
		sbuf3 = prow;
		s1 = (mlib_d64 *)sbuf1;
		s2 = (mlib_d64 *)sbuf2;
		s3 = (mlib_d64 *)sbuf3;
		d1 = *s1;
		d2 = *s2;
		d3 = *s3;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_3 = *(s3 + 1);
			out0 = out1 = rnd;
			CONV_AU(d1, k12);
			CONV_AL(d2, k34);
			CONV_AU(d3, k78);
			d22 = vis_faligndata(d2, d_2);
			dt_2 = vis_faligndata(d_2, d2);
			d21 = vis_faligndata(d1, d_1);
			dt_1 = vis_faligndata(d_1, d1);
			d23 = vis_faligndata(d3, d_3);
			dt_3 = vis_faligndata(d_3, d3);
			CONV_AL(d21, k12);
			CONV_AU(d22, k56);
			CONV_AL(d23, k78);
			d31 = vis_faligndata(d21, dt_1);
			d32 = vis_faligndata(d22, dt_2);
			d33 = vis_faligndata(d23, dt_3);
			CONV_AU(d31, k34);
			CONV_AL(d32, k56);
			CONV_AU(d33, k9);
			(*ddst++) =
			    vis_freg_pair(vis_fpack16(out0), vis_fpack16(out1));
			d1 = d_1;
			d2 = d_2;
			d3 = d_3;
			s1++;
			s2++;
			s3++;
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
							vis_faligndata(sd,
							sd), da);
						    da += nchannel;

						    if ((mlib_addr)da <=
							(mlib_addr)dend) {
							vis_st_u8(sd =
							    vis_faligndata
							    (sd, sd), da);
							da += nchannel;

							if ((mlib_addr)da <=
							    (mlib_addr)dend) {
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
mlib_conv3x3nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchan, amask;

	if (mlib_ImageConvVersion(3, 3, scalef_expon, MLIB_BYTE) == 0)
		return mlib_c_conv3x3nw_u8(dst, src, kernel, scalef_expon,
		    cmask);

	nchan = mlib_ImageGetChannels(src);
	amask = (1 << nchan) - 1;
	cmask &= amask;

	if (cmask == amask) {
		return (mlib_conv3x3_8nw_f(dst, src, kernel, scalef_expon));
	}

	if ((nchan == 4) && (cmask == 0xE || cmask == 0x7)) {
		return mlib_v_conv3x3_8nw_4(dst, src, kernel, scalef_expon,
		    cmask);
	} else {
		return mlib_v_conv3x3_8nw_mask(dst, src, kernel, scalef_expon,
		    cmask);
	}
}

/* *********************************************************** */
