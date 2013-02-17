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

#pragma ident	"@(#)mlib_v_ImageSConv5x5_u16nw.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_sconv5x5nw_u16 - separable convolve of an image with a 5x5 kernel
 *                            edge = MLIB_EDGE_DST_NO_WRITE
 *
 * SYNOPSIS
 *      mlib_status mlib_sconv5x5nw_u16(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      const mlib_s32   *hkernel,
 *                                      const mlib_s32   *vkernel,
 *                                      mlib_s32         scale,
 *                                      mlib_s32         cmask)
 *
 *
 * ARGUMENT
 *      dst       Pointer to the destination image data structure
 *      src       Pointer to the source image data structure
 *      hkernel   Pointer to 5 elements of the horizontal kernel
 *      vkernel   Pointer to 5 elements of the vertical kernel
 *      scale     The integer coefficients are assumed to be equal
 *                to some floating point value < 1.0, multiplied by
 *                a scalefactor that is a power of 2. Values are
 *                divided by this scalefactor before being output.
 *      cmask     Channel mask to indicate the channels to be convolved.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *
 * DESCRIPTION
 *      2-D separable convolution, 5x5 kernel.
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

static mlib_status mlib_v_sconv5x5_u16nw_1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_sconv5x5_u16nw_2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_sconv5x5_u16nw_3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_sconv5x5_u16nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

static mlib_status mlib_v_sconv5x5_u16nw_mask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

/* *********************************************************** */

mlib_status
mlib_sconv5x5nw_u16(
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
			    mlib_v_sconv5x5_u16nw_1(dst, src, hkernel, vkernel,
			    scalef_expon);
			break;
		case 2:
			res =
			    mlib_v_sconv5x5_u16nw_2(dst, src, hkernel, vkernel,
			    scalef_expon);
			break;
		case 3:
			res =
			    mlib_v_sconv5x5_u16nw_3(dst, src, hkernel, vkernel,
			    scalef_expon);
			break;
		case 4:
			res =
			    mlib_v_sconv5x5_u16nw_4(dst, src, hkernel, vkernel,
			    scalef_expon, cmask);
			break;
		default:
			res = MLIB_FAILURE;
			break;
		}
	} else {
		res =
		    mlib_v_sconv5x5_u16nw_mask(dst, src, hkernel, vkernel,
		    scalef_expon, cmask);
	}

	return (res);
}

/* *********************************************************** */

typedef union
{
	mlib_d64 value;
	struct
	{
		mlib_u16 ushort0, ushort1, ushort2, ushort3;
	} forshort;
} type_mlib_d64;

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS()                                \
	adr_src = (mlib_u8 *)mlib_ImageGetData(src);            \
	adr_dst = (mlib_u8 *)mlib_ImageGetData(dst);            \
	slb = mlib_ImageGetStride(src);                         \
	dlb = mlib_ImageGetStride(dst);                         \
	dh = mlib_ImageGetHeight(dst);                          \
	dw = mlib_ImageGetWidth(dst)

/* *********************************************************** */

#define	LOAD_KERNEL_INTO_FLOAT()                                        \
/*                                                                      \
 * load vertical kernel values into doubles                             \
 */                                                                     \
	v0 =                                                            \
	vis_to_double_dup((vkernel[0] & 0xFFFF0000) | ((vkernel[0] >>   \
	    16) & 0xFFFF));                                             \
	v1 =                                                            \
	vis_to_double_dup((vkernel[1] & 0xFFFF0000) | ((vkernel[1] >>   \
	    16) & 0xFFFF));                                             \
	v2 =                                                            \
	vis_to_double_dup((vkernel[2] & 0xFFFF0000) | ((vkernel[2] >>   \
	    16) & 0xFFFF));                                             \
	v3 =                                                            \
	vis_to_double_dup((vkernel[3] & 0xFFFF0000) | ((vkernel[3] >>   \
	    16) & 0xFFFF));                                             \
	v4 =                                                            \
	vis_to_double_dup((vkernel[4] & 0xFFFF0000) | ((vkernel[4] >>   \
	    16) & 0xFFFF));                                             \
/*                                                                      \
 * load horizontal kernel values into doubles                           \
 */                                                                     \
	h0 =                                                            \
	vis_to_double_dup((hkernel[0] & 0xFFFF0000) | ((hkernel[0] >>   \
	    16) & 0xFFFF));                                             \
	h1 =                                                            \
	vis_to_double_dup((hkernel[1] & 0xFFFF0000) | ((hkernel[1] >>   \
	    16) & 0xFFFF));                                             \
	h2 =                                                            \
	vis_to_double_dup((hkernel[2] & 0xFFFF0000) | ((hkernel[2] >>   \
	    16) & 0xFFFF));                                             \
	h3 =                                                            \
	vis_to_double_dup((hkernel[3] & 0xFFFF0000) | ((hkernel[3] >>   \
	    16) & 0xFFFF));                                             \
	h4 =                                                            \
	vis_to_double_dup((hkernel[4] & 0xFFFF0000) | ((hkernel[4] >>   \
	    16) & 0xFFFF))

/* *********************************************************** */

#define	PREPARE_INTERM_BUFFERS()                                          \
	buff_src =                                                        \
	    (mlib_d64 *)__mlib_malloc(6 * buf_slb * sizeof (mlib_d64));   \
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
	ddst = dbuf;                                            \
	da = d_a;                                               \
	dend = da + (dw - 1);                                   \
	prow = sbuf1;                                           \
	sbuf1 = sbuf2;                                          \
	sbuf2 = sbuf3;                                          \
	sbuf3 = sbuf4;                                          \
	sbuf4 = sbuf5;                                          \
	sbuf5 = prow;                                           \
	s1 = sbuf1;                                             \
	s2 = sbuf2;                                             \
	s3 = sbuf3;                                             \
	s4 = sbuf4;                                             \
	s5 = sbuf5

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

#define	VCONV(dsrc0, dsrc1, dsrc2, dsrc3, dsrc4, sum0)          \
	dsrc0 = vis_fxor(dsrc0, mask8000);                      \
	dsrc1 = vis_fxor(dsrc1, mask8000);                      \
	dsrc2 = vis_fxor(dsrc2, mask8000);                      \
	dsrc3 = vis_fxor(dsrc3, mask8000);                      \
	dsrc4 = vis_fxor(dsrc4, mask8000);                      \
	sum0 = vis_fmul8sux16(dsrc0, v0);                       \
	tmp0 = vis_fmul8ulx16(dsrc0, v0);                       \
	tmp2 = vis_fmul8sux16(dsrc1, v1);                       \
	sum0 = vis_fpadd16(sum0, tmp0);                         \
	tmp1 = vis_fmul8ulx16(dsrc1, v1);                       \
	sum0 = vis_fpadd16(sum0, tmp2);                         \
	tmp0 = vis_fmul8sux16(dsrc2, v2);                       \
	sum0 = vis_fpadd16(sum0, tmp1);                         \
	tmp2 = vis_fmul8ulx16(dsrc2, v2);                       \
	sum0 = vis_fpadd16(sum0, tmp0);                         \
	tmp1 = vis_fmul8sux16(dsrc3, v3);                       \
	sum0 = vis_fpadd16(sum0, tmp2);                         \
	tmp0 = vis_fmul8ulx16(dsrc3, v3);                       \
	sum0 = vis_fpadd16(sum0, tmp1);                         \
	tmp2 = vis_fmul8sux16(dsrc4, v4);                       \
	sum0 = vis_fpadd16(sum0, tmp0);                         \
	tmp1 = vis_fmul8ulx16(dsrc4, v4);                       \
	sum0 = vis_fpadd16(sum0, tmp2);                         \
	sum0 = vis_fpadd16(sum0, tmp1)

/* *********************************************************** */

#define	HCONV_START(s1_shift0, s2_shift0)                       \
	out0 = vis_fmul8sux16(s1_shift0, h0);                   \
	htmp0 = vis_fmul8ulx16(s1_shift0, h0);                  \
	out1 = vis_fmul8sux16(s2_shift0, h0);                   \
	out0 = vis_fpadd16(out0, htmp0);                        \
	htmp1 = vis_fmul8ulx16(s2_shift0, h0);                  \
	out1 = vis_fpadd16(out1, htmp1)

/* *********************************************************** */

#define	HCONV_START_MASK(s1_shift0)                             \
	out0 = vis_fmul8sux16(s1_shift0, h0);                   \
	htmp0 = vis_fmul8ulx16(s1_shift0, h0);                  \
	out0 = vis_fpadd16(out0, htmp0)

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
	src2_shift2 = vis_faligndata(src2_shift1, src3_shift1);   \
	src3_shift2 = vis_faligndata(src3_shift1, src4_shift1);   \
	src1_shift3 = vis_faligndata(src1_shift2, src2_shift2);   \
	src2_shift3 = vis_faligndata(src2_shift2, src3_shift2)

/* *********************************************************** */

#define	SHIFTPRODS_2()                                          \
	src2_shift1 = vis_faligndata(acc1, acc2);               \
	src3_shift1 = vis_faligndata(acc2, acc3)

/* *********************************************************** */

#define	SHIFTPRODS_3()                                            \
	src3_shift1 = vis_faligndata(acc2, acc3);                 \
	src4_shift1 = vis_faligndata(acc3, acc4);                 \
	src2_shift2 = vis_faligndata(src2_shift1, src3_shift1);   \
	src3_shift2 = vis_faligndata(src3_shift1, src4_shift1);   \
	src1_shift3 = vis_faligndata(src1_shift2, src2_shift2);   \
	src2_shift3 = vis_faligndata(src2_shift2, src3_shift2)

/* *********************************************************** */

#define	SHIFTPRODS_MASK()                                         \
	src2_shift1 = vis_faligndata(acc1, acc2);                 \
	src1_shift2 = vis_faligndata(src1_shift1, src2_shift1);   \
	src3_shift1 = vis_faligndata(acc2, acc2);                 \
	src2_shift2 = vis_faligndata(src2_shift1, src3_shift1);   \
	src1_shift3 = vis_faligndata(src1_shift2, src2_shift2)

/* *********************************************************** */

#define	SAVE_RES(res, out0)                                     \
	out00 = vis_fmuld8sux16(f_scale, vis_read_hi(out0));    \
	out10 = vis_fmuld8sux16(f_scale, vis_read_lo(out0));    \
	out00 = vis_fpadd32(out00, koff);                       \
	out10 = vis_fpadd32(out10, koff);                       \
	out00 = vis_fpackfix_pair(out00, out10);                \
	res = vis_fxor(out00, mask8000)

/* *********************************************************** */

#define	KER_OFFSET()                                                     \
	mlib_d64 koff, mask8000 = vis_to_double_dup(0x80008000);         \
	mlib_s32 off;                                                    \
	                                                                 \
	tmp0 =                                                           \
	    (mlib_d64)hkernel[0] + hkernel[1] + hkernel[2] +             \
	    hkernel[3] + hkernel[4];                                     \
	tmp1 =                                                           \
	    (mlib_d64)vkernel[0] + vkernel[1] + vkernel[2] +             \
	    vkernel[3] + vkernel[4];                                     \
	tmp0 = tmp0 * tmp1 * ((1.0 / (1u << 30)) * (1.0 / (1u << 5)));   \
	if (scalef_expon < 33) {                                         \
	    gsr_scale = 66 - 2 * scalef_expon;                           \
	    f_scale = vis_to_float(0x40004000);                          \
	    off = (mlib_s32)tmp0 - (1 << (2 * scalef_expon - 35));       \
	} else {                                                         \
	    gsr_scale = 2;                                               \
	    f_scale = vis_to_float(0x10001000);                          \
	    off =                                                        \
		(mlib_s32)(tmp0 * 0.25) - (1 << (2 * scalef_expon -      \
		37));                                                    \
	}                                                                \
	                                                                 \
	koff = vis_to_double_dup(off);                                   \
	vis_write_gsr((gsr_scale << 3) | 2)

/* *********************************************************** */

mlib_status
mlib_v_sconv5x5_u16nw_1(
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
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf4, *sbuf5;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out00, out10;
	mlib_d64 out1;
	mlib_d64 acc0, acc1, acc2, acc3;
	mlib_d64 tmp0, tmp1, tmp2, htmp0, htmp1;
	mlib_d64 d1, d2, d3, d4, d5;

/* data */
	mlib_d64 d6, d7, d8, d9, d10;
	mlib_d64 src1_shift1, src2_shift1, src3_shift1, src4_shift1;
	mlib_d64 src1_shift2, src2_shift2, src3_shift2;
	mlib_d64 src1_shift3, src2_shift3;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 h0, h1, h2, h3, h4;
	mlib_d64 v0, v1, v2, v3, v4;
	mlib_f32 f_scale;
	mlib_s32 emask;
	mlib_s32 gsr_scale, i, j;

	KER_OFFSET();

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	dw *= 2;

	buf_slb = (dw + 48) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 8;
	dh -= 4;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	sa4 = sa3 + slb;
	d_a = adr_dst + 2 * (dlb + 2);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(8);

	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(8);

	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(8);

	PREPARE_TO_LOAD_LINE(sbuf5, sa3);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(8);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf5, sa4);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(8);

		vis_alignaddr(s1, 2);
		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		d6 = s1[1];
		d7 = s2[1];
		d8 = s3[1];
		d9 = s4[1];
		d10 = s5[1];
		VCONV(d1, d2, d3, d4, d5, acc0);
		VCONV(d6, d7, d8, d9, d10, acc1);
		d1 = s1[2];
		d2 = s2[2];
		d3 = s3[2];
		d4 = s4[2];
		d5 = s5[2];
		d6 = s1[3];
		d7 = s2[3];
		d8 = s3[3];
		d9 = s4[3];
		d10 = s5[3];
		s1 += 4;
		s2 += 4;
		s3 += 4;
		s4 += 4;
		s5 += 4;

		src1_shift1 = vis_faligndata(acc0, acc1);
		src2_shift1 = vis_faligndata(acc1, acc1);
		src1_shift2 = vis_faligndata(src1_shift1, src2_shift1);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 16) {
			VCONV(d1, d2, d3, d4, d5, acc2);
			VCONV(d6, d7, d8, d9, d10, acc3);

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
			d6 = s1[1];
			d7 = s2[1];
			d8 = s3[1];
			d9 = s4[1];
			d10 = s5[1];

			HCONV_SRC2(src2_shift1, h1);
			HCONV_SRC1(src1_shift2, h2);
			HCONV_SRC2(src2_shift2, h2);
			HCONV_SRC1(src1_shift3, h3);
			HCONV_SRC2(src2_shift3, h3);

			SAVE_RES((*ddst++), out0);
			SAVE_RES((*ddst++), out1);

			acc0 = acc2;
			acc1 = acc3;
			src1_shift1 = src3_shift1;
			src1_shift2 = src3_shift2;
			s1 += 2;
			s2 += 2;
			s3 += 2;
			s4 += 2;
			s5 += 2;
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
mlib_v_sconv5x5_u16nw_2(
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
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf4, *sbuf5;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out00, out10;
	mlib_d64 out1;
	mlib_d64 acc0, acc1, acc2, acc3;
	mlib_d64 tmp0, tmp1, tmp2, htmp0, htmp1;
	mlib_d64 d1, d2, d3, d4, d5;

/* data */
	mlib_d64 d6, d7, d8, d9, d10;
	mlib_d64 src1_shift1, src2_shift1, src3_shift1;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 h0, h1, h2, h3, h4;
	mlib_d64 v0, v1, v2, v3, v4;
	mlib_f32 f_scale;
	mlib_s32 emask;
	mlib_s32 gsr_scale, i, j;

	KER_OFFSET();

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	dw *= 4;

	buf_slb = (dw + 48) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 16;
	dh -= 4;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	sa4 = sa3 + slb;
	d_a = adr_dst + 2 * (dlb + 4);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(16);

	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(16);

	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(16);

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
		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		d6 = s1[1];
		d7 = s2[1];
		d8 = s3[1];
		d9 = s4[1];
		d10 = s5[1];
		VCONV(d1, d2, d3, d4, d5, acc0);
		VCONV(d6, d7, d8, d9, d10, acc1);
		d1 = s1[2];
		d2 = s2[2];
		d3 = s3[2];
		d4 = s4[2];
		d5 = s5[2];
		d6 = s1[3];
		d7 = s2[3];
		d8 = s3[3];
		d9 = s4[3];
		d10 = s5[3];
		s1 += 4;
		s2 += 4;
		s3 += 4;
		s4 += 4;
		s5 += 4;

		src1_shift1 = vis_faligndata(acc0, acc1);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 16) {
			VCONV(d1, d2, d3, d4, d5, acc2);
			VCONV(d6, d7, d8, d9, d10, acc3);

			HCONV_START(acc0, acc1);
			HCONV_SRC1(src1_shift1, h1);
			HCONV_SRC1(acc1, h2);

			HCONV_SRC2(acc2, h2);
			HCONV_SRC1(acc2, h4);
			HCONV_SRC2(acc3, h4);

			SHIFTPRODS_2();

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			d6 = s1[1];
			d7 = s2[1];
			d8 = s3[1];
			d9 = s4[1];
			d10 = s5[1];

			HCONV_SRC2(src2_shift1, h1);
			HCONV_SRC1(src2_shift1, h3);
			HCONV_SRC2(src3_shift1, h3);

			SAVE_RES((*ddst++), out0);
			SAVE_RES((*ddst++), out1);

			acc0 = acc2;
			acc1 = acc3;
			src1_shift1 = src3_shift1;
			s1 += 2;
			s2 += 2;
			s3 += 2;
			s4 += 2;
			s5 += 2;
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
mlib_v_sconv5x5_u16nw_3(
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
	mlib_u8 *sa, *sa1, *sa2, *sa3, *sa4;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf4, *sbuf5;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out00, out10;
	mlib_d64 out1;
	mlib_d64 acc0, acc1, acc2, acc3, acc4, acc5;
	mlib_d64 tmp0, tmp1, tmp2, htmp0, htmp1;
	mlib_d64 d1, d2, d3, d4, d5;

/* data */
	mlib_d64 d6, d7, d8, d9, d10;
	mlib_d64 src1_shift1, src2_shift1, src3_shift1, src4_shift1;
	mlib_d64 src1_shift2, src2_shift2, src3_shift2;
	mlib_d64 src1_shift3, src2_shift3;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 h0, h1, h2, h3, h4;
	mlib_d64 v0, v1, v2, v3, v4;
	mlib_f32 f_scale;
	mlib_s32 emask;
	mlib_s32 gsr_scale, i, j;

	KER_OFFSET();

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	dw *= 6;

	buf_slb = (dw + 64) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 24;
	dh -= 4;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	sa4 = sa3 + slb;
	d_a = adr_dst + 2 * (dlb + 6);

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

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf5, sa4);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(24);

		vis_alignaddr(s1, 6);
		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		d6 = s1[1];
		d7 = s2[1];
		d8 = s3[1];
		d9 = s4[1];
		d10 = s5[1];
		VCONV(d1, d2, d3, d4, d5, acc0);
		VCONV(d6, d7, d8, d9, d10, acc1);
		d1 = s1[2];
		d2 = s2[2];
		d3 = s3[2];
		d4 = s4[2];
		d5 = s5[2];
		d6 = s1[3];
		d7 = s2[3];
		d8 = s3[3];
		d9 = s4[3];
		d10 = s5[3];
		VCONV(d1, d2, d3, d4, d5, acc2);
		VCONV(d6, d7, d8, d9, d10, acc3);
		d1 = s1[4];
		d2 = s2[4];
		d3 = s3[4];
		d4 = s4[4];
		d5 = s5[4];
		d6 = s1[5];
		d7 = s2[5];
		d8 = s3[5];
		d9 = s4[5];
		d10 = s5[5];
		s1 += 6;
		s2 += 6;
		s3 += 6;
		s4 += 6;
		s5 += 6;

		src1_shift1 = vis_faligndata(acc0, acc1);
		src2_shift1 = vis_faligndata(acc1, acc2);
		src1_shift2 = vis_faligndata(src1_shift1, src2_shift1);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 16) {
			VCONV(d1, d2, d3, d4, d5, acc4);
			VCONV(d6, d7, d8, d9, d10, acc5);

			HCONV_START(acc0, acc1);
			HCONV_SRC1(src1_shift1, h1);
			HCONV_SRC2(src2_shift1, h1);
			HCONV_SRC1(src1_shift2, h2);

			HCONV_SRC1(acc3, h4);
			HCONV_SRC2(acc4, h4);

			SHIFTPRODS_3();

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			d6 = s1[1];
			d7 = s2[1];
			d8 = s3[1];
			d9 = s4[1];
			d10 = s5[1];

			HCONV_SRC2(src2_shift2, h2);
			HCONV_SRC1(src1_shift3, h3);
			HCONV_SRC2(src2_shift3, h3);

			SAVE_RES((*ddst++), out0);
			SAVE_RES((*ddst++), out1);

			acc0 = acc2;
			acc1 = acc3;
			acc2 = acc4;
			acc3 = acc5;
			src1_shift1 = src3_shift1;
			src1_shift2 = src3_shift2;
			src2_shift1 = src4_shift1;
			s1 += 2;
			s2 += 2;
			s3 += 2;
			s4 += 2;
			s5 += 2;
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
mlib_v_sconv5x5_u16nw_4(
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

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_d64 *sbuf4, *sbuf5;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out00, out10;
	mlib_d64 out1;
	mlib_d64 acc0, acc1, acc2, acc3, acc4, acc5;
	mlib_d64 tmp0, tmp1, tmp2, htmp0, htmp1;
	mlib_d64 d1, d2, d3, d4, d5;

/* data */
	mlib_d64 d6, d7, d8, d9, d10;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1, sd00;
	mlib_d64 h0, h1, h2, h3, h4;
	mlib_d64 v0, v1, v2, v3, v4;
	mlib_f32 f_scale;
	mlib_s32 emask, cmask1;
	mlib_s32 gsr_scale, i, j;

	KER_OFFSET();

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	dw *= 8;

	buf_slb = (dw + 64) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 32;
	dh -= 4;

	sa = adr_src;
	sa1 = sa + slb;
	sa2 = sa1 + slb;
	sa3 = sa2 + slb;
	sa4 = sa3 + slb;
	d_a = adr_dst + 2 * (dlb + 8);

/* load interm. src buff */
	PREPARE_TO_LOAD_LINE(sbuf2, sa);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(32);

	PREPARE_TO_LOAD_LINE(sbuf3, sa1);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(32);

	PREPARE_TO_LOAD_LINE(sbuf4, sa2);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(32);

	PREPARE_TO_LOAD_LINE(sbuf5, sa3);
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(32);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

		PREPARE_TO_LOAD_LINE(sbuf5, sa4);
#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER_NF(32);

		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		d6 = s1[1];
		d7 = s2[1];
		d8 = s3[1];
		d9 = s4[1];
		d10 = s5[1];
		VCONV(d1, d2, d3, d4, d5, acc0);
		VCONV(d6, d7, d8, d9, d10, acc1);
		d1 = s1[2];
		d2 = s2[2];
		d3 = s3[2];
		d4 = s4[2];
		d5 = s5[2];
		d6 = s1[3];
		d7 = s2[3];
		d8 = s3[3];
		d9 = s4[3];
		d10 = s5[3];
		VCONV(d1, d2, d3, d4, d5, acc2);
		VCONV(d6, d7, d8, d9, d10, acc3);
		d1 = s1[4];
		d2 = s2[4];
		d3 = s3[4];
		d4 = s4[4];
		d5 = s5[4];
		d6 = s1[5];
		d7 = s2[5];
		d8 = s3[5];
		d9 = s4[5];
		d10 = s5[5];
		s1 += 6;
		s2 += 6;
		s3 += 6;
		s4 += 6;
		s5 += 6;

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 16) {
			VCONV(d1, d2, d3, d4, d5, acc4);
			VCONV(d6, d7, d8, d9, d10, acc5);

			HCONV_START(acc0, acc1);
			HCONV_SRC1(acc1, h1);
			HCONV_SRC2(acc2, h1);
			HCONV_SRC1(acc2, h2);

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			d6 = s1[1];
			d7 = s2[1];
			d8 = s3[1];
			d9 = s4[1];
			d10 = s5[1];

			HCONV_SRC2(acc3, h2);
			HCONV_SRC1(acc3, h3);
			HCONV_SRC2(acc4, h3);
			HCONV_SRC1(acc4, h4);
			HCONV_SRC2(acc5, h4);

			SAVE_RES((*ddst++), out0);
			SAVE_RES((*ddst++), out1);

			acc0 = acc2;
			acc1 = acc3;
			acc2 = acc4;
			acc3 = acc5;
			s1 += 2;
			s2 += 2;
			s3 += 2;
			s4 += 2;
			s5 += 2;
		}

		ddst = dbuf;
		cmask1 = ((cmask & 0xf) << 4) + (cmask & 0xf);

/* prepare the destination addresses */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		i = (mlib_addr)dp - (mlib_addr)da;
		cmask1 >>= ((-i) >> 1);
		ddst = vis_alignaddr(ddst, i);

/* generate edge mask for the start point */
		emask = vis_edge16(da, dend);

		sd1 = ddst[0];

		if (emask != 0xf) {
			sd0 = sd1;
			sd1 = ddst[1];
			sd0 = vis_faligndata(sd0, sd1);
			vis_pst_16(sd0, dp++, emask & cmask1);
			ddst++;
			i += 8;
		}
#pragma pipeloop(0)
		for (; i <= (dw - 8); i += 8) {
			sd0 = sd1;
			sd1 = ddst[1];
			sd00 = vis_faligndata(sd0, sd1);
			vis_pst_16(sd00, dp++, cmask1);
			ddst++;
		}

		if (i < dw) {
			sd0 = vis_faligndata(sd1, ddst[1]);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd0, dp, emask & cmask1);
		}

		sa4 = sa4 + slb;
		d_a += dlb;
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MAIN_ITERATION()                                        \
	VCONV(d1, d2, d3, d4, d5, acc2);                        \
	HCONV_START_MASK(acc0);                                 \
	HCONV_SRC1(src1_shift1, h1);                            \
	HCONV_SRC1(acc1, h4);                                   \
	SHIFTPRODS_MASK();                                      \
	d1 = s1[0];                                             \
	d2 = s2[0];                                             \
	d3 = s3[0];                                             \
	d4 = s4[0];                                             \
	d5 = s5[0];                                             \
	HCONV_SRC1(src1_shift2, h2);                            \
	HCONV_SRC1(src1_shift3, h3);                            \
	SAVE_RES(str.value, out0);                              \
	acc0 = acc1;                                            \
	acc1 = acc2;                                            \
	src1_shift1 = src2_shift1;                              \
	s1++;                                                   \
	s2++;                                                   \
	s3++;                                                   \
	s4++;                                                   \
	s5++

/* *********************************************************** */

mlib_status
mlib_v_sconv5x5_u16nw_mask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u16 *da, *d_a;

/* pointers to src, dst data */
	mlib_u16 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u16 *sa, *sa1, *sa2, *sa3, *sa4, *sa5, *sa_5;

/* pointers to rows in interm. src buf */
	mlib_u16 *buff_src, *sbuf1, *sbuf2, *sbuf3, *prow;

/* pointers to rows in interm. src buf */
	mlib_u16 *sbuf4, *sbuf5, *s_buf1;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2, *s3, *s4, *s5;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out00, out10;
	mlib_d64 acc0, acc1, acc2;
	mlib_d64 tmp0, tmp1, tmp2, htmp0, htmp1;

/* data */
	mlib_d64 d1, d2, d3, d4, d5;
	mlib_d64 src1_shift1, src2_shift1, src3_shift1;
	mlib_d64 src1_shift2, src2_shift2, src1_shift3;
	mlib_d64 h0, h1, h2, h3, h4;
	mlib_d64 v0, v1, v2, v3, v4;
	mlib_f32 f_scale;
	int gsr_scale, i, j, nchannel, chan, testchan;
	mlib_u16 t1, t2, t3, t4, t5, t6, t7, t8;
	type_mlib_d64 str;

	KER_OFFSET();

	adr_src = (mlib_u16 *)mlib_ImageGetData(src);
	adr_dst = (mlib_u16 *)mlib_ImageGetData(dst);
	nchannel = mlib_ImageGetChannels(src);
	slb = mlib_ImageGetStride(src) >> 1;
	dlb = mlib_ImageGetStride(dst) >> 1;
	dh = mlib_ImageGetHeight(dst);
	dw = mlib_ImageGetWidth(dst);

	LOAD_KERNEL_INTO_FLOAT();

/* buf_slb - 8-byte aligned */
	buf_slb = ((dw << 1) + 26) & (~7);
/* alloc. interm. src and dst buffer */
	buff_src =
	    (mlib_u16 *)__mlib_malloc(5 * buf_slb * sizeof (mlib_u8) + 24);

	if (buff_src == NULL)
		return (MLIB_FAILURE);
	dw -= 4;
/* edge - no write */
	dh -= 4;

	buf_slb >>= 1;

	sbuf1 = (mlib_u16 *)((mlib_addr)(buff_src + 8) & (~7));
	sbuf2 = sbuf1 + buf_slb;
	sbuf3 = sbuf2 + buf_slb;
	sbuf4 = sbuf3 + buf_slb;
	sbuf5 = sbuf4 + buf_slb;

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
		sa_5 = sa5 = sa4 + slb;
		d_a = adr_dst + 2 * (dlb + nchannel) + chan;

/* load interm. src buff */
		for (i = 0, j = 0; j < (dw + 4); i += nchannel, j++) {
			sbuf1[j] = sa4[i];
			sbuf2[j] = sa[i];
			sbuf3[j] = sa1[i];
			sbuf4[j] = sa2[i];
			sbuf5[j] = sa3[i];
		}

#pragma pipeloop(0)
		for (j = 0; j < dh - 1; j++) {
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

			d1 = s1[0];
			d2 = s2[0];
			d3 = s3[0];
			d4 = s4[0];
			d5 = s5[0];
			VCONV(d1, d2, d3, d4, d5, acc0);
			d1 = s1[1];
			d2 = s2[1];
			d3 = s3[1];
			d4 = s4[1];
			d5 = s5[1];
			VCONV(d1, d2, d3, d4, d5, acc1);
			d1 = s1[2];
			d2 = s2[2];
			d3 = s3[2];
			d4 = s4[2];
			d5 = s5[2];
			s1 += 3;
			s2 += 3;
			s3 += 3;
			s4 += 3;
			s5 += 3;

			src1_shift1 = vis_faligndata(acc0, acc1);

/* first iteration - no write data to dst (only in union) */
			t1 = *sa_5;
			sa_5 += nchannel;
			t2 = *sa_5;
			sa_5 += nchannel;
			t3 = *sa_5;
			sa_5 += nchannel;
			t4 = *sa_5;
			sa_5 += nchannel;
			(*s_buf1++) = t1;
			(*s_buf1++) = t2;
			(*s_buf1++) = t3;
			(*s_buf1++) = t4;

			MAIN_ITERATION();

#pragma pipeloop(0)
			for (i = 4; i < dw; i += 4) {
/*
 * in each iteration store result from prev. iterat.
 * and load data for processing next row
 */
				t1 = *sa_5;
				sa_5 += nchannel;
				t2 = *sa_5;
				sa_5 += nchannel;
				t3 = *sa_5;
				sa_5 += nchannel;
				t4 = *sa_5;
				sa_5 += nchannel;
				t5 = str.forshort.ushort0;
				t6 = str.forshort.ushort1;
				t7 = str.forshort.ushort2;
				t8 = str.forshort.ushort3;
				(*s_buf1++) = t1;
				(*s_buf1++) = t2;
				(*s_buf1++) = t3;
				(*s_buf1++) = t4;
				*da = t5;
				da += nchannel;
				*da = t6;
				da += nchannel;
				*da = t7;
				da += nchannel;
				*da = t8;
				da += nchannel;

				MAIN_ITERATION();
			}

			(*s_buf1++) = vis_ld_u16_nf(sa_5);
			sa_5 += nchannel;
			(*s_buf1++) = vis_ld_u16_nf(sa_5);
			sa_5 += nchannel;
			(*s_buf1++) = vis_ld_u16_nf(sa_5);
			sa_5 += nchannel;
			(*s_buf1++) = vis_ld_u16_nf(sa_5);

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = str.forshort.ushort0;
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = str.forshort.ushort1;
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = str.forshort.ushort2;
				da += nchannel;
			}

			if ((mlib_addr)da <= (mlib_addr)dend) {
				*da = str.forshort.ushort3;
			}

			sa_5 = sa5 = sa5 + slb;
			d_a += dlb;
		}

/* process last row - no need to load data */

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

		d1 = s1[0];
		d2 = s2[0];
		d3 = s3[0];
		d4 = s4[0];
		d5 = s5[0];
		VCONV(d1, d2, d3, d4, d5, acc0);
		d1 = s1[1];
		d2 = s2[1];
		d3 = s3[1];
		d4 = s4[1];
		d5 = s5[1];
		VCONV(d1, d2, d3, d4, d5, acc1);
		d1 = s1[2];
		d2 = s2[2];
		d3 = s3[2];
		d4 = s4[2];
		d5 = s5[2];
		s1 += 3;
		s2 += 3;
		s3 += 3;
		s4 += 3;
		s5 += 3;

		src1_shift1 = vis_faligndata(acc0, acc1);

/* first iteration - no write data to dst (only in union) */

		MAIN_ITERATION();

#pragma pipeloop(0)
		for (i = 4; i < dw; i += 4) {
/* in each iteration store result from prev. iterat. */
			t5 = str.forshort.ushort0;
			t6 = str.forshort.ushort1;
			t7 = str.forshort.ushort2;
			t8 = str.forshort.ushort3;
			*da = t5;
			da += nchannel;
			*da = t6;
			da += nchannel;
			*da = t7;
			da += nchannel;
			*da = t8;
			da += nchannel;

			MAIN_ITERATION();
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = str.forshort.ushort0;
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = str.forshort.ushort1;
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = str.forshort.ushort2;
			da += nchannel;
		}

		if ((mlib_addr)da <= (mlib_addr)dend) {
			*da = str.forshort.ushort3;
		}
	}

	__mlib_free(buff_src);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
