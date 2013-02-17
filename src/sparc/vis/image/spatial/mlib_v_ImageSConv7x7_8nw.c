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

#pragma ident	"@(#)mlib_v_ImageSConv7x7_8nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_sconv7x7_8nw - separable convolve of an image with a 7x7 kernel
 *                          edge = MLIB_EDGE_DST_NO_WRITE
 *
 * SYNOPSIS
 *      mlib_status mlib_sconv7x7nw_u8(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_s32   *hkernel,
 *                                     const mlib_s32   *vkernel,
 *                                     mlib_s32         scale,
 *                                     mlib_s32         cmask)
 *
 *
 * ARGUMENT
 *      dst       Pointer to the destination image data structure
 *      src       Pointer to the source image data structure
 *      hkernel   Pointer to 7 elements of the horizontal kernel
 *      vkernel   Pointer to 7 elements of the vertical kernel
 *      scale     The integer coefficients are assumed to be equal
 *                to some floating point value < 1.0, multiplied by
 *                a scalefactor that is a power of 2. Values are
 *                divided by this scalefactor before being output.
 *      cmask     Channel mask to indicate the channels to be convolved.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *
 * DESCRIPTION
 *      2-D separable convolution, 7x7 kernel.
 *
 *      The src and the dst must be the same type and have same number
 *      of channels (1, 2, 3, or 4). The unselected channels are not
 *      overwritten. If both src and dst have just one channel,
 *      cmask is ignored.
 *
 */

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageSConv.h>

/* *********************************************************** */

static mlib_status mlib_v_sconv7x7_8nw_1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_sconv7x7_8nw_2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_sconv7x7_8nw_3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_sconv7x7_8nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

static mlib_status mlib_v_sconv7x7_8nw_mask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

/* *********************************************************** */

mlib_status
mlib_sconv7x7nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchannel, chan, cmask1, i;
	mlib_status res;

	cmask1 = cmask;
	nchannel = mlib_ImageGetChannels(src);
	chan = 0;
	for (i = nchannel - 1; i >= 0; i--, cmask1 >>= 1)
		if ((cmask1 & 1) == 1)
			chan++;

	if ((nchannel == chan) || ((nchannel == 4) && (chan == 3))) {
		switch (nchannel) {
		case 1:
			res =
			    mlib_v_sconv7x7_8nw_1(dst, src, hkernel, vkernel,
			    scalef_expon);
			break;
		case 2:
			res =
			    mlib_v_sconv7x7_8nw_2(dst, src, hkernel, vkernel,
			    scalef_expon);
			break;
		case 3:
			res =
			    mlib_v_sconv7x7_8nw_3(dst, src, hkernel, vkernel,
			    scalef_expon);
			break;
		case 4:
			res =
			    mlib_v_sconv7x7_8nw_4(dst, src, hkernel, vkernel,
			    scalef_expon, cmask);
			break;
		default:
			res = MLIB_FAILURE;
			break;
		}
	} else
		res =
		    mlib_v_sconv7x7_8nw_mask(dst, src, hkernel, vkernel,
		    scalef_expon, cmask);
	return (res);
}

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
/*                                                                       \
 * load horizontal kernel values into doubles                            \
 */                                                                      \
	h0 =                                                             \
	vis_to_double_dup((hkernel[0] & 0xFFFF0000) | ((hkernel[0] >>    \
	    16) & 0xFFFF));                                              \
	h1 =                                                             \
	vis_to_double_dup((hkernel[1] & 0xFFFF0000) | ((hkernel[1] >>    \
	    16) & 0xFFFF));                                              \
	h2 =                                                             \
	vis_to_double_dup((hkernel[2] & 0xFFFF0000) | ((hkernel[2] >>    \
	    16) & 0xFFFF));                                              \
	h3 =                                                             \
	vis_to_double_dup((hkernel[3] & 0xFFFF0000) | ((hkernel[3] >>    \
	    16) & 0xFFFF));                                              \
	h4 =                                                             \
	vis_to_double_dup((hkernel[4] & 0xFFFF0000) | ((hkernel[4] >>    \
	    16) & 0xFFFF));                                              \
	h5 =                                                             \
	vis_to_double_dup((hkernel[5] & 0xFFFF0000) | ((hkernel[5] >>    \
	    16) & 0xFFFF));                                              \
	h6 =                                                             \
	vis_to_double_dup((hkernel[6] & 0xFFFF0000) | ((hkernel[6] >>    \
	    16) & 0xFFFF));                                              \
/*                                                                       \
 * load vertical kernel values into floats                               \
 */                                                                      \
	v0 =                                                             \
	vis_to_float((vkernel[0] & 0xFFFF0000) | ((vkernel[0] >> 16) &   \
	    0xFFFF));                                                    \
	v1 =                                                             \
	vis_to_float((vkernel[1] & 0xFFFF0000) | ((vkernel[1] >> 16) &   \
	    0xFFFF));                                                    \
	v2 =                                                             \
	vis_to_float((vkernel[2] & 0xFFFF0000) | ((vkernel[2] >> 16) &   \
	    0xFFFF));                                                    \
	v3 =                                                             \
	vis_to_float((vkernel[3] & 0xFFFF0000) | ((vkernel[3] >> 16) &   \
	    0xFFFF));                                                    \
	v4 =                                                             \
	vis_to_float((vkernel[4] & 0xFFFF0000) | ((vkernel[4] >> 16) &   \
	    0xFFFF));                                                    \
	v5 =                                                             \
	vis_to_float((vkernel[5] & 0xFFFF0000) | ((vkernel[5] >> 16) &   \
	    0xFFFF));                                                    \
	v6 =                                                             \
	vis_to_float((vkernel[6] & 0xFFFF0000) | ((vkernel[6] >> 16) &   \
	    0xFFFF))

/* *********************************************************** */

#define	PREPARE_INTERM_BUFFERS()                                          \
	buff_src =                                                        \
	    (mlib_d64 *)__mlib_malloc(8 * buf_slb * sizeof (mlib_d64));   \
	if (buff_src == NULL)                                             \
	    return (MLIB_FAILURE);                                        \
	sbuf1 = buff_src;                                                 \
	sbuf2 = sbuf1 + buf_slb;                                          \
	sbuf3 = sbuf2 + buf_slb;                                          \
	sbuf4 = sbuf3 + buf_slb;                                          \
	sbuf5 = sbuf4 + buf_slb;                                          \
	sbuf6 = sbuf5 + buf_slb;                                          \
	sbuf7 = sbuf6 + buf_slb;                                          \
	dbuf = sbuf7 + buf_slb

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
	sbuf5 = sbuf6;                                          \
	sbuf6 = sbuf7;                                          \
	sbuf7 = prow;                                           \
	s1 = (mlib_d64 *)sbuf1;                                 \
	s2 = (mlib_d64 *)sbuf2;                                 \
	s3 = (mlib_d64 *)sbuf3;                                 \
	s4 = (mlib_d64 *)sbuf4;                                 \
	s5 = (mlib_d64 *)sbuf5;                                 \
	s6 = (mlib_d64 *)sbuf6;                                 \
	s7 = (mlib_d64 *)sbuf7

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

#define	VCONV(dsrc0, dsrc1, dsrc2, dsrc3, dsrc4, dsrc5,         \
	dsrc6, sum0, sum1)                                      \
	sum0 = vis_fmul8x16au(vis_read_hi(dsrc0), v0);          \
	sum1 = vis_fmul8x16au(vis_read_lo(dsrc0), v0);          \
	tmp0 = vis_fmul8x16au(vis_read_hi(dsrc1), v1);          \
	tmp1 = vis_fmul8x16au(vis_read_lo(dsrc1), v1);          \
	sum0 = vis_fpadd16(sum0, tmp0);                         \
	sum1 = vis_fpadd16(sum1, tmp1);                         \
	tmp0 = vis_fmul8x16au(vis_read_hi(dsrc2), v2);          \
	tmp1 = vis_fmul8x16au(vis_read_lo(dsrc2), v2);          \
	sum0 = vis_fpadd16(sum0, tmp0);                         \
	sum1 = vis_fpadd16(sum1, tmp1);                         \
	tmp0 = vis_fmul8x16au(vis_read_hi(dsrc3), v3);          \
	tmp1 = vis_fmul8x16au(vis_read_lo(dsrc3), v3);          \
	sum0 = vis_fpadd16(sum0, tmp0);                         \
	sum1 = vis_fpadd16(sum1, tmp1);                         \
	tmp0 = vis_fmul8x16au(vis_read_hi(dsrc4), v4);          \
	tmp1 = vis_fmul8x16au(vis_read_lo(dsrc4), v4);          \
	sum0 = vis_fpadd16(sum0, tmp0);                         \
	sum1 = vis_fpadd16(sum1, tmp1);                         \
	tmp0 = vis_fmul8x16au(vis_read_hi(dsrc5), v5);          \
	tmp1 = vis_fmul8x16au(vis_read_lo(dsrc5), v5);          \
	sum0 = vis_fpadd16(sum0, tmp0);                         \
	sum1 = vis_fpadd16(sum1, tmp1);                         \
	tmp0 = vis_fmul8x16au(vis_read_hi(dsrc6), v6);          \
	tmp1 = vis_fmul8x16au(vis_read_lo(dsrc6), v6);          \
	sum0 = vis_fpadd16(sum0, tmp0);                         \
	sum1 = vis_fpadd16(sum1, tmp1)

/* *********************************************************** */

#define	HCONV_START(s1_shift0, s2_shift0)                       \
	out0 = vis_fmul8sux16(s1_shift0, h0);                   \
	htmp0 = vis_fmul8ulx16(s1_shift0, h0);                  \
	out1 = vis_fmul8sux16(s2_shift0, h0);                   \
	out0 = vis_fpadd16(out0, htmp0);                        \
	htmp1 = vis_fmul8ulx16(s2_shift0, h0);                  \
	out1 = vis_fpadd16(out1, htmp1)

/* *********************************************************** */

#define	HCONV_SRC1(s1_shift, koeff)                             \
	htmp0 = vis_fmul8sux16(s1_shift, koeff);                \
	out0 = vis_fpadd16(out0, htmp0);                        \
	htmp1 = vis_fmul8ulx16(s1_shift, koeff);                \
	out0 = vis_fpadd16(out0, htmp1)

/* *********************************************************** */

#define	HCONV_SRC2(s2_shift, koeff)                             \
	htmp0 = vis_fmul8sux16(s2_shift, koeff);                \
	out1 = vis_fpadd16(out1, htmp0);                        \
	htmp1 = vis_fmul8ulx16(s2_shift, koeff);                \
	out1 = vis_fpadd16(out1, htmp1)

/* *********************************************************** */

#define	SHIFTPRODS_1()                                            \
	src2_shift1 = vis_faligndata(acc1, acc2);                 \
	src3_shift1 = vis_faligndata(acc2, acc3);                 \
	src4_shift1 = vis_faligndata(acc3, acc3);                 \
	src1_shift2 = vis_faligndata(src1_shift1, src2_shift1);   \
	src2_shift2 = vis_faligndata(src2_shift1, src3_shift1);   \
	src3_shift2 = vis_faligndata(src3_shift1, src4_shift1);   \
	src1_shift3 = vis_faligndata(src1_shift2, src2_shift2);   \
	src2_shift3 = vis_faligndata(src2_shift2, src3_shift2)

/* *********************************************************** */

#define	SHIFTPRODS_2()                                          \
	src1_shift1 = vis_faligndata(acc0, acc1);               \
	src2_shift1 = vis_faligndata(acc1, acc2);               \
	src3_shift1 = vis_faligndata(acc2, acc3);               \
	src4_shift1 = vis_faligndata(acc3, acc4)

/* *********************************************************** */

#define	SHIFTPRODS_3()                                            \
	src2_shift1 = vis_faligndata(acc1, acc2);                 \
	src3_shift1 = vis_faligndata(acc2, acc3);                 \
	src4_shift1 = vis_faligndata(acc3, acc4);                 \
	src5_shift1 = vis_faligndata(acc4, acc5);                 \
	src6_shift1 = vis_faligndata(acc5, acc6);                 \
	src1_shift2 = vis_faligndata(src1_shift1, src2_shift1);   \
	src2_shift2 = vis_faligndata(src2_shift1, src3_shift1);   \
	src3_shift2 = vis_faligndata(src3_shift1, src4_shift1);   \
	src4_shift2 = vis_faligndata(src4_shift1, src5_shift1);   \
	src5_shift2 = vis_faligndata(src5_shift1, src6_shift1);   \
	src1_shift3 = vis_faligndata(src1_shift2, src2_shift2);   \
	src2_shift3 = vis_faligndata(src2_shift2, src3_shift2)

/* *********************************************************** */

mlib_status
mlib_v_sconv7x7_8nw_1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4, *sa5, *sa6;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf4, *sbuf5, *sbuf6, *sbuf7;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5, *s6, *s7;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1;
	mlib_d64 acc0, acc1, acc2, acc3;
	mlib_d64 tmp0, tmp1, htmp0, htmp1;

/* data */
	mlib_d64 d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 src1_shift1, src2_shift1, src3_shift1, src4_shift1;
	mlib_d64 src1_shift2, src2_shift2, src3_shift2;
	mlib_d64 src1_shift3, src2_shift3;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 h0, h1, h2, h3, h4, h5, h6;
	mlib_f32 v0, v1, v2, v3, v4, v5, v6;
	mlib_s32 emask;
	mlib_s32 gsr_scale, i, j;

	gsr_scale = 63 - (scalef_expon << 1);
	vis_write_gsr((gsr_scale << 3));

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	buf_slb = (dw + 24) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 6;
	dh -= 6;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	sa4 = sa3 + slb;
	sa5 = sa4 + slb;
	sa6 = sa5 + slb;
	d_a = adr_dst + 3 * (dlb + 1);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(6);

	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(6);

	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(6);

	PREPARE_TO_LOAD_LINE(sbuf5, sa3);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(6);

	PREPARE_TO_LOAD_LINE(sbuf6, sa4);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(6);

	PREPARE_TO_LOAD_LINE(sbuf7, sa5);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(6);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf7, sa6);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(6);

		vis_alignaddr(s1, 2);
		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		d6 = s6[0];
		d7 = s7[0];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc0, acc1);
		d1 = s1[1];
		d2 = s2[1];
		d3 = s3[1];
		d4 = s4[1];
		d5 = s5[1];
		d6 = s6[1];
		d7 = s7[1];
		s1 += 2;
		s2 += 2;
		s3 += 2;
		s4 += 2;
		s5 += 2;
		s6 += 2;
		s7 += 2;

		src1_shift1 = vis_faligndata(acc0, acc1);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			VCONV(d1, d2, d3, d4, d5, d6, d7, acc2, acc3);

			HCONV_START(acc0, acc1);
			HCONV_SRC1(src1_shift1, h1);
			HCONV_SRC1(acc1, h4);
			HCONV_SRC2(acc2, h4);

			SHIFTPRODS_1();

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			d6 = s6[0];
			d7 = s7[0];

			HCONV_SRC2(src2_shift1, h1);
			HCONV_SRC1(src1_shift2, h2);
			HCONV_SRC2(src2_shift2, h2);
			HCONV_SRC1(src1_shift3, h3);
			HCONV_SRC2(src2_shift3, h3);
			HCONV_SRC1(src2_shift1, h5);
			HCONV_SRC2(src3_shift1, h5);
			HCONV_SRC1(src2_shift2, h6);
			HCONV_SRC2(src3_shift2, h6);
			(*ddst++) = vis_fpack16_pair(out0, out1);

			acc0 = acc2;
			acc1 = acc3;
			src1_shift1 = src3_shift1;

			s1++;
			s2++;
			s3++;
			s4++;
			s5++;
			s6++;
			s7++;
		}

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();

		sa6 = sa6 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_sconv7x7_8nw_2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4, *sa5, *sa6;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf4, *sbuf5, *sbuf6, *sbuf7;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5, *s6, *s7;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1;
	mlib_d64 acc0, acc1, acc2, acc3, acc4, acc5;
	mlib_d64 tmp0, tmp1, htmp0, htmp1;

/* data */
	mlib_d64 d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 src1_shift1, src2_shift1, src3_shift1, src4_shift1;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 h0, h1, h2, h3, h4, h5, h6;
	mlib_f32 v0, v1, v2, v3, v4, v5, v6;
	mlib_s32 emask;
	mlib_s32 gsr_scale, i, j;

	gsr_scale = 63 - (scalef_expon << 1);
	vis_write_gsr((gsr_scale << 3));

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	buf_slb = (2 * dw + 32) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 6;
	dw *= 2;
	dh -= 6;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	sa4 = sa3 + slb;
	sa5 = sa4 + slb;
	sa6 = sa5 + slb;
	d_a = adr_dst + 3 * (dlb + 2);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(12);

	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(12);

	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(12);

	PREPARE_TO_LOAD_LINE(sbuf5, sa3);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(12);

	PREPARE_TO_LOAD_LINE(sbuf6, sa4);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(12);

	PREPARE_TO_LOAD_LINE(sbuf7, sa5);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(12);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf7, sa6);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(12);

		vis_alignaddr(s1, 4);
		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		d6 = s6[0];
		d7 = s7[0];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc0, acc1);
		d1 = s1[1];
		d2 = s2[1];
		d3 = s3[1];
		d4 = s4[1];
		d5 = s5[1];
		d6 = s6[1];
		d7 = s7[1];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc2, acc3);
		d1 = s1[2];
		d2 = s2[2];
		d3 = s3[2];
		d4 = s4[2];
		d5 = s5[2];
		d6 = s6[2];
		d7 = s7[2];
		s1 += 3;
		s2 += 3;
		s3 += 3;
		s4 += 3;
		s5 += 3;
		s6 += 3;
		s7 += 3;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			VCONV(d1, d2, d3, d4, d5, d6, d7, acc4, acc5);

			HCONV_START(acc0, acc1);
			HCONV_SRC1(acc1, h2);
			HCONV_SRC2(acc2, h2);
			HCONV_SRC1(acc2, h4);
			HCONV_SRC2(acc3, h4);
			HCONV_SRC1(acc3, h6);
			HCONV_SRC2(acc4, h6);

			SHIFTPRODS_2();

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			d6 = s6[0];
			d7 = s7[0];

			HCONV_SRC1(src1_shift1, h1);
			HCONV_SRC2(src2_shift1, h1);
			HCONV_SRC1(src2_shift1, h3);
			HCONV_SRC2(src3_shift1, h3);
			HCONV_SRC1(src3_shift1, h5);
			HCONV_SRC2(src4_shift1, h5);
			(*ddst++) = vis_fpack16_pair(out0, out1);

			acc0 = acc2;
			acc1 = acc3;
			acc2 = acc4;
			acc3 = acc5;

			s1++;
			s2++;
			s3++;
			s4++;
			s5++;
			s6++;
			s7++;
		}

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();

		sa6 = sa6 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_sconv7x7_8nw_3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4, *sa5, *sa6;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf4, *sbuf5, *sbuf6, *sbuf7;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5, *s6, *s7;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1;
	mlib_d64 acc0, acc1, acc2, acc3, acc4, acc5, acc6, acc7;
	mlib_d64 tmp0, tmp1, htmp0, htmp1;

/* data */
	mlib_d64 d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 src1_shift1, src2_shift1, src3_shift1, src4_shift1;
	mlib_d64 src5_shift1, src6_shift1;
	mlib_d64 src1_shift2, src2_shift2, src3_shift2;
	mlib_d64 src4_shift2, src5_shift2;
	mlib_d64 src1_shift3, src2_shift3;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 h0, h1, h2, h3, h4, h5, h6;
	mlib_f32 v0, v1, v2, v3, v4, v5, v6;
	mlib_s32 emask;
	mlib_s32 gsr_scale, i, j;

	gsr_scale = 63 - (scalef_expon << 1);
	vis_write_gsr((gsr_scale << 3));

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	buf_slb = (3 * dw + 40) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 6;
	dw *= 3;
	dh -= 6;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	sa4 = sa3 + slb;
	sa5 = sa4 + slb;
	sa6 = sa5 + slb;
	d_a = adr_dst + 3 * (dlb + 3);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(18);

	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(18);

	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(18);

	PREPARE_TO_LOAD_LINE(sbuf5, sa3);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(18);

	PREPARE_TO_LOAD_LINE(sbuf6, sa4);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(18);

	PREPARE_TO_LOAD_LINE(sbuf7, sa5);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(18);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf7, sa6);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(18);

		vis_alignaddr(s1, 6);
		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		d6 = s6[0];
		d7 = s7[0];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc0, acc1);
		d1 = s1[1];
		d2 = s2[1];
		d3 = s3[1];
		d4 = s4[1];
		d5 = s5[1];
		d6 = s6[1];
		d7 = s7[1];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc2, acc3);
		d1 = s1[2];
		d2 = s2[2];
		d3 = s3[2];
		d4 = s4[2];
		d5 = s5[2];
		d6 = s6[2];
		d7 = s7[2];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc4, acc5);
		d1 = s1[3];
		d2 = s2[3];
		d3 = s3[3];
		d4 = s4[3];
		d5 = s5[3];
		d6 = s6[3];
		d7 = s7[3];
		s1 += 4;
		s2 += 4;
		s3 += 4;
		s4 += 4;
		s5 += 4;
		s6 += 4;
		s7 += 4;

		src1_shift1 = vis_faligndata(acc0, acc1);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			VCONV(d1, d2, d3, d4, d5, d6, d7, acc6, acc7);

			HCONV_START(acc0, acc1);
			HCONV_SRC1(src1_shift1, h1);
			HCONV_SRC1(acc3, h4);
			HCONV_SRC2(acc4, h4);

			SHIFTPRODS_3();

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			d6 = s6[0];
			d7 = s7[0];

			HCONV_SRC2(src2_shift1, h1);
			HCONV_SRC1(src1_shift2, h2);
			HCONV_SRC2(src2_shift2, h2);
			HCONV_SRC1(src1_shift3, h3);
			HCONV_SRC2(src2_shift3, h3);
			HCONV_SRC1(src4_shift1, h5);
			HCONV_SRC2(src5_shift1, h5);
			HCONV_SRC1(src4_shift2, h6);
			HCONV_SRC2(src5_shift2, h6);
			(*ddst++) = vis_fpack16_pair(out0, out1);

			acc0 = acc2;
			acc1 = acc3;
			acc2 = acc4;
			acc3 = acc5;
			acc4 = acc6;
			acc5 = acc7;
			src1_shift1 = src3_shift1;

			s1++;
			s2++;
			s3++;
			s4++;
			s5++;
			s6++;
			s7++;
		}

		PREPARE_TO_COPY_INTERM_BUF_TO_DST();

#pragma pipeloop(0)
		COPY_INTERM_BUF_TO_DST();
		COPY_TAIL();

		sa6 = sa6 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_sconv7x7_8nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4, *sa5, *sa6;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf4, *sbuf5, *sbuf6, *sbuf7;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5, *s6, *s7;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1;
	mlib_d64 acc0, acc1, acc2, acc3, acc4, acc5, acc6, acc7;
	mlib_d64 tmp0, tmp1, htmp0, htmp1;

/* data */
	mlib_d64 d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1, sd00;
	mlib_d64 h0, h1, h2, h3, h4, h5, h6;
	mlib_f32 v0, v1, v2, v3, v4, v5, v6;
	mlib_s32 emask, cmask1;
	mlib_s32 gsr_scale, i, j;

	gsr_scale = 63 - (scalef_expon << 1);
	vis_write_gsr((gsr_scale << 3));

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	buf_slb = (4 * dw + 40) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 6;
	dw *= 4;
	dh -= 6;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	sa4 = sa3 + slb;
	sa5 = sa4 + slb;
	sa6 = sa5 + slb;
	d_a = adr_dst + 3 * (dlb + 4);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(24);

	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(24);

	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(24);

	PREPARE_TO_LOAD_LINE(sbuf5, sa3);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(24);

	PREPARE_TO_LOAD_LINE(sbuf6, sa4);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(24);

	PREPARE_TO_LOAD_LINE(sbuf7, sa5);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(24);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf7, sa6);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(24);

		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		d6 = s6[0];
		d7 = s7[0];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc0, acc1);
		d1 = s1[1];
		d2 = s2[1];
		d3 = s3[1];
		d4 = s4[1];
		d5 = s5[1];
		d6 = s6[1];
		d7 = s7[1];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc2, acc3);
		d1 = s1[2];
		d2 = s2[2];
		d3 = s3[2];
		d4 = s4[2];
		d5 = s5[2];
		d6 = s6[2];
		d7 = s7[2];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc4, acc5);
		d1 = s1[3];
		d2 = s2[3];
		d3 = s3[3];
		d4 = s4[3];
		d5 = s5[3];
		d6 = s6[3];
		d7 = s7[3];
		s1 += 4;
		s2 += 4;
		s3 += 4;
		s4 += 4;
		s5 += 4;
		s6 += 4;
		s7 += 4;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			VCONV(d1, d2, d3, d4, d5, d6, d7, acc6, acc7);

			HCONV_START(acc0, acc1);
			HCONV_SRC1(acc1, h1);
			HCONV_SRC2(acc2, h1);
			HCONV_SRC1(acc2, h2);
			HCONV_SRC2(acc3, h2);
			HCONV_SRC1(acc3, h3);
			HCONV_SRC2(acc4, h3);

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			d6 = s6[0];
			d7 = s7[0];

			HCONV_SRC1(acc4, h4);
			HCONV_SRC2(acc5, h4);
			HCONV_SRC1(acc5, h5);
			HCONV_SRC2(acc6, h5);
			HCONV_SRC1(acc6, h6);
			HCONV_SRC2(acc7, h6);
			(*ddst++) = vis_fpack16_pair(out0, out1);

			acc0 = acc2;
			acc1 = acc3;
			acc2 = acc4;
			acc3 = acc5;
			acc4 = acc6;
			acc5 = acc7;

			s1++;
			s2++;
			s3++;
			s4++;
			s5++;
			s6++;
			s7++;
		}

		ddst = dbuf;
		cmask1 = ((cmask & 0xf) << 4) + (cmask & 0xf);
		cmask1 = (cmask1 << 8) + (cmask1);

/* prepare the destination addresses */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		i = (mlib_addr)dp - (mlib_addr)da;
		cmask1 >>= (-i);

/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);
		ddst = vis_alignaddr(ddst, i);

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

		sa6 = sa6 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_sconv7x7_8nw_mask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u8 *da, *d_a;

/* pointers to src, dst data */
	mlib_u8 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4;

/* pointers to src rows */
	mlib_u8 *sa5, *sa6, *sa7, *sa_7;

/* pointers to rows in interm. src buf */
	mlib_u8 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_u8 *sbuf4, *sbuf5, *sbuf6, *sbuf7, *s_buf1;

/* pointer to row in interm. dst buf */
	mlib_u8 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5, *s6, *s7;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_d64 *buff_src1;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1;
	mlib_d64 acc0, acc1, acc2, acc3;
	mlib_d64 tmp0, tmp1, htmp0, htmp1;

/* data */
	mlib_d64 d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 src1_shift1, src2_shift1, src3_shift1, src4_shift1;
	mlib_d64 src1_shift2, src2_shift2, src3_shift2;
	mlib_d64 src1_shift3, src2_shift3;
	mlib_d64 sd;
	mlib_d64 h0, h1, h2, h3, h4, h5, h6;
	mlib_f32 v0, v1, v2, v3, v4, v5, v6;
	mlib_s32 gsr_scale, i, j, nchannel, chan, testchan;
	mlib_u8 t0, t1, t2, t3, t4, t5, t6, t7;

	gsr_scale = 63 - (scalef_expon << 1);
	vis_write_gsr((gsr_scale << 3));

	GET_SRC_DST_PARAMETERS();
	nchannel = mlib_ImageGetChannels(src);
	LOAD_KERNEL_INTO_FLOAT();

	buf_slb = (dw + 24) >> 3;
/* alloc. interm. src and dst buffer */
	buff_src1 = (mlib_d64 *)__mlib_malloc(8 * buf_slb * sizeof (mlib_d64));

	if (buff_src1 == NULL)
		return (MLIB_FAILURE);
	buff_src = (mlib_u8 *)buff_src1;
	buf_slb <<= 3;

	dw -= 6;
	dh -= 6;

	sbuf1 = buff_src;
	sbuf2 = sbuf1 + buf_slb;
	sbuf3 = sbuf2 + buf_slb;
	sbuf4 = sbuf3 + buf_slb;
	sbuf5 = sbuf4 + buf_slb;
	sbuf6 = sbuf5 + buf_slb;
	sbuf7 = sbuf6 + buf_slb;
	dbuf = sbuf7 + buf_slb;

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
		sa4 = sa3 + slb;
		sa5 = sa4 + slb;
		sa6 = sa5 + slb;
		sa_7 = sa7 = sa6 + slb;
		d_a = adr_dst + 3 * (dlb + nchannel) + chan;

/* load interm. src buff */
		for (i = 0, j = 0; j < (dw + 6); i += nchannel, j++) {
			sbuf1[j] = sa6[i];
			sbuf2[j] = sa[i];
			sbuf3[j] = sa1[i];
			sbuf4[j] = sa2[i];
			sbuf5[j] = sa3[i];
			sbuf6[j] = sa4[i];
			sbuf7[j] = sa5[i];
		}

#pragma pipeloop(0)
		for (j = 0; j < dh - 1; j++) {
			LOOP_INI();
			dend = da + (dw - 1) * nchannel;
			s_buf1 = sbuf1;

			vis_alignaddr(s1, 2);

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			d6 = s6[0];
			d7 = s7[0];
			VCONV(d1, d2, d3, d4, d5, d6, d7, acc0, acc1);
			d1 = s1[1];
			d2 = s2[1];
			d3 = s3[1];
			d4 = s4[1];
			d5 = s5[1];
			d6 = s6[1];
			d7 = s7[1];
			s1 += 2;
			s2 += 2;
			s3 += 2;
			s4 += 2;
			s5 += 2;
			s6 += 2;
			s7 += 2;

			src1_shift1 = vis_faligndata(acc0, acc1);

/* in each iteration load data for processing next row */
#pragma pipeloop(0)
			for (i = 0; i < dw; i += 8) {
				t0 = *sa_7;
				sa_7 += nchannel;
				t1 = *sa_7;
				sa_7 += nchannel;
				t2 = *sa_7;
				sa_7 += nchannel;
				t3 = *sa_7;
				sa_7 += nchannel;
				t4 = *sa_7;
				sa_7 += nchannel;
				t5 = *sa_7;
				sa_7 += nchannel;
				t6 = *sa_7;
				sa_7 += nchannel;
				t7 = *sa_7;
				sa_7 += nchannel;

				VCONV(d1, d2, d3, d4, d5, d6, d7, acc2, acc3);

				HCONV_START(acc0, acc1);
				HCONV_SRC1(src1_shift1, h1);
				(*s_buf1++) = t0;
				HCONV_SRC1(acc1, h4);
				(*s_buf1++) = t1;
				HCONV_SRC2(acc2, h4);

				SHIFTPRODS_1();

				d1 = s1[0];
				d2 = s2[0];
				d3 = s3[0];
				d4 = s4[0];
				d5 = s5[0];
				d6 = s6[0];
				d7 = s7[0];

				HCONV_SRC2(src2_shift1, h1);
				(*s_buf1++) = t2;
				HCONV_SRC1(src1_shift2, h2);
				(*s_buf1++) = t3;
				HCONV_SRC2(src2_shift2, h2);
				(*s_buf1++) = t4;
				HCONV_SRC1(src1_shift3, h3);
				(*s_buf1++) = t5;
				HCONV_SRC2(src2_shift3, h3);
				(*s_buf1++) = t6;
				HCONV_SRC1(src2_shift1, h5);
				(*s_buf1++) = t7;
				HCONV_SRC2(src3_shift1, h5);
				HCONV_SRC1(src2_shift2, h6);
				HCONV_SRC2(src3_shift2, h6);
				(*ddst++) = vis_fpack16_pair(out0, out1);

				acc0 = acc2;
				acc1 = acc3;
				src1_shift1 = src3_shift1;

				s1++;
				s2++;
				s3++;
				s4++;
				s5++;
				s6++;
				s7++;
			}

			(*s_buf1++) = vis_ld_u8_nf(sa_7);
			sa_7 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_7);
			sa_7 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_7);
			sa_7 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_7);
			sa_7 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_7);
			sa_7 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_7);
			sa_7 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_7);
			sa_7 += nchannel;
			(*s_buf1++) = vis_ld_u8_nf(sa_7);

			ddst = (mlib_d64 *)dbuf;
			vis_alignaddr(s1, 1);

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

							if ((mlib_addr)da <=
							    (mlib_addr)dend) {
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

			sa_7 = sa7 = sa7 + slb;
			d_a += dlb;
		}

/* process last row - no need to load data */
		LOOP_INI();
		dend = da + (dw - 1) * nchannel;

		vis_alignaddr(s1, 2);

		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		d6 = s6[0];
		d7 = s7[0];
		VCONV(d1, d2, d3, d4, d5, d6, d7, acc0, acc1);
		d1 = s1[1];
		d2 = s2[1];
		d3 = s3[1];
		d4 = s4[1];
		d5 = s5[1];
		d6 = s6[1];
		d7 = s7[1];
		s1 += 2;
		s2 += 2;
		s3 += 2;
		s4 += 2;
		s5 += 2;
		s6 += 2;
		s7 += 2;

		src1_shift1 = vis_faligndata(acc0, acc1);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 8) {
			VCONV(d1, d2, d3, d4, d5, d6, d7, acc2, acc3);

			HCONV_START(acc0, acc1);
			HCONV_SRC1(src1_shift1, h1);
			HCONV_SRC1(acc1, h4);
			HCONV_SRC2(acc2, h4);

			SHIFTPRODS_1();

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			d6 = s6[0];
			d7 = s7[0];

			HCONV_SRC2(src2_shift1, h1);
			HCONV_SRC1(src1_shift2, h2);
			HCONV_SRC2(src2_shift2, h2);
			HCONV_SRC1(src1_shift3, h3);
			HCONV_SRC2(src2_shift3, h3);
			HCONV_SRC1(src2_shift1, h5);
			HCONV_SRC2(src3_shift1, h5);
			HCONV_SRC1(src2_shift2, h6);
			HCONV_SRC2(src3_shift2, h6);
			(*ddst++) = vis_fpack16_pair(out0, out1);

			acc0 = acc2;
			acc1 = acc3;
			src1_shift1 = src3_shift1;

			s1++;
			s2++;
			s3++;
			s4++;
			s5++;
			s6++;
			s7++;
		}

		ddst = (mlib_d64 *)dbuf;
		vis_alignaddr(s1, 1);

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
						vis_st_u8(sd = vis_faligndata
						    (sd, sd), da);
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
						    }
						}
					    }
					}
				}
			}
		}
	}

	__mlib_free(buff_src1);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
