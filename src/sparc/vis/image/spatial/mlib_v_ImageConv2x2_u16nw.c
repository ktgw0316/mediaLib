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

#pragma ident	"@(#)mlib_v_ImageConv2x2_u16nw.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_conv2x2nw_u16 - convolve an U16 image with a 2x2 kernel
 *                           (edge = no write)
 *
 * SYNOPSIS
 *      mlib_status mlib_conv2x2nw_u16(mlib_image       *src,
 *                                     const mlib_image *dst,
 *                                     const mlib_s32   *kernel,
 *                                     mlib_s32         scalef_expon,
 *                                     mlib_s32         cmask)
 *
 * ARGUMENT
 *      src           Pointer to structure of source image
 *      dst           Pointer to structure of destination image
 *      kernel        Convolution kernel
 *      scalef_expon  The scaling factor to convert the input integer
 *                    coefficients into floating-point coefficients:
 *                    floating-point coeff. = integer coeff. * 2^(-scale)
 *      cmask         Channel mask to indicate the channels to be convolved.
 *                    Each bit of which represents a channel in the image. The
 *                    channels corresponded to 1 bits are those to be processed.
 *
 * DESCRIPTION
 *      A 2-D convolution (2x2 kernel) for 16-bit images.
 */

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>

/* *********************************************************** */

static mlib_status mlib_v_conv2x2_u16nw_1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_conv2x2_u16nw_2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_conv2x2_u16nw_3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_conv2x2_u16nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon);

static mlib_status mlib_v_conv2x2_u16nw_mask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

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
	adr_src = (mlib_u16 *)mlib_ImageGetData(src);           \
	adr_dst = (mlib_u16 *)mlib_ImageGetData(dst);           \
	slb = mlib_ImageGetStride(src) >> 1;                    \
	dlb = mlib_ImageGetStride(dst) >> 1;                    \
	dh = mlib_ImageGetHeight(dst);                          \
	dw = mlib_ImageGetWidth(dst)

/* *********************************************************** */

#define	LOAD_KERNEL_INTO_FLOAT()                                           \
	{                                                                  \
	    mlib_s32 k[4], shift1, rnd_val = 0, ker_sum = 0;               \
	                                                                   \
	    if (scalef_expon == 31) {                                      \
	                                                                   \
/*                                                                         \
 * prevent overflow                                                        \
 */                                                                        \
		shift1 = 17;                                               \
		rnd_val = (1 << 16);                                       \
		scalef_expon--;                                            \
	    } else {                                                       \
		shift1 = 16;                                               \
	    }                                                              \
	    for (i = 0; i < 4; i++) {                                      \
		k[i] = (kernel[i] + rnd_val) >> shift1;                    \
		ker_sum += k[i];                                           \
	    }                                                              \
	    k1 = vis_to_float((k[0] & 0xFFFF) | (k[0] << 16));             \
	    k2 = vis_to_float((k[1] & 0xFFFF) | (k[1] << 16));             \
	    k3 = vis_to_float((k[2] & 0xFFFF) | (k[2] << 16));             \
	    k4 = vis_to_float((k[3] & 0xFFFF) | (k[3] << 16));             \
	    ker_sum = (ker_sum << 15) - (0x8000 << (scalef_expon - 16));   \
	    ker_off = vis_to_double_dup(ker_sum);                          \
	}

/* *********************************************************** */

#define	PREPARE_INTERM_BUFFERS()                                           \
	buff_src =                                                         \
	    (mlib_d64 *)__mlib_malloc(3 * buf_slb * sizeof (mlib_d64));    \
	if (buff_src == NULL)                                              \
	    return (MLIB_FAILURE);                                         \
	sbuf1 = buff_src;                                                  \
	sbuf2 = sbuf1 + buf_slb;                                           \
	dbuf = sbuf2 + buf_slb

/* *********************************************************** */

#define	LOAD_LINE_INTO_BUFFER(p_buf, p_line, n)                             \
	if (!((mlib_s32)p_line & 7)) {                                      \
		for (i = 0; i < (dw + n + 3)/4; i++) {                      \
			p_buf[i] = ((mlib_d64 *)p_line)[i];                 \
		}                                                           \
	} else {                                                            \
		mlib_ImageCopy_na((void*)p_line, (void*)p_buf, 2*(dw + n)); \
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
	i = ((mlib_u16 *)dp - (mlib_u16 *)da);                  \
	ddst = vis_alignaddr(ddst, 2 * i);                      \
/*                                                              \
 * generate edge mask for the start point                       \
 */                                                             \
	emask = vis_edge16(da, dend);                           \
	sd1 = ddst[0];                                          \
	if (emask != 0xf) {                                     \
	    sd0 = sd1;                                          \
	    sd1 = ddst[1];                                      \
	    sd0 = vis_faligndata(sd0, sd1);                     \
	    vis_pst_16(sd0, dp++, emask);                       \
	    ddst++;                                             \
	    i += 4;                                             \
	}

/* *********************************************************** */

#define	COPY_INTERM_BUF_TO_DST()                                \
	for (; i <= (dw - 4); i += 4) {                         \
	    sd0 = sd1;                                          \
	    sd1 = ddst[1];                                      \
	    (*dp++) = vis_faligndata(sd0, sd1);                 \
	    ddst++;                                             \
	}

/* *********************************************************** */

#define	COPY_TAIL()                                             \
	if (i < dw) {                                           \
	    sd0 = vis_faligndata(sd1, ddst[1]);                 \
	    emask = vis_edge16(dp, dend);                       \
	    vis_pst_16(sd0, dp, emask);                         \
	}

/* *********************************************************** */

#define	CONV_16_BEGIN(dsrc, ka)                                 \
	tmp0 = vis_fmuld8sux16(vis_read_hi(dsrc), ka);          \
	tmp1 = vis_fmuld8ulx16(vis_read_hi(dsrc), ka);          \
	tmp2 = vis_fmuld8sux16(vis_read_lo(dsrc), ka);          \
	tmp3 = vis_fmuld8ulx16(vis_read_lo(dsrc), ka);          \
	out0 = vis_fpadd32(ker_off, tmp0);                      \
	out1 = vis_fpadd32(ker_off, tmp2);                      \
	out0 = vis_fpadd32(out0, tmp1);                         \
	out1 = vis_fpadd32(out1, tmp3)

/* *********************************************************** */

#define	CONV_16(dsrc, ka)                                       \
	tmp0 = vis_fmuld8sux16(vis_read_hi(dsrc), ka);          \
	tmp1 = vis_fmuld8ulx16(vis_read_hi(dsrc), ka);          \
	tmp2 = vis_fmuld8sux16(vis_read_lo(dsrc), ka);          \
	tmp3 = vis_fmuld8ulx16(vis_read_lo(dsrc), ka);          \
	tmp0 = vis_fpadd32(tmp0, tmp1);                         \
	out0 = vis_fpadd32(out0, tmp0);                         \
	tmp2 = vis_fpadd32(tmp2, tmp3);                         \
	out1 = vis_fpadd32(out1, tmp2)

/* *********************************************************** */

#define	CONV_16_MIX(dsrc1, dsrc2, ka)                           \
	tmp0 = vis_fmuld8sux16(vis_read_lo(dsrc1), ka);         \
	tmp1 = vis_fmuld8ulx16(vis_read_lo(dsrc1), ka);         \
	tmp2 = vis_fmuld8sux16(vis_read_hi(dsrc2), ka);         \
	tmp3 = vis_fmuld8ulx16(vis_read_hi(dsrc2), ka);         \
	tmp0 = vis_fpadd32(tmp0, tmp1);                         \
	out0 = vis_fpadd32(out0, tmp0);                         \
	tmp2 = vis_fpadd32(tmp2, tmp3);                         \
	out1 = vis_fpadd32(out1, tmp2)

/* *********************************************************** */

mlib_status
mlib_v_conv2x2_u16nw_1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon)
{
/* pointers to dst row */
	mlib_u16 *da, *d_a;

/* pointers to src, dst data */
	mlib_u16 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u16 *sa, *sa1;

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
	mlib_f32 k1, k2, k3, k4;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, tmp2, tmp3;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 emask;
	int gsr_scale, i, j;
	mlib_d64 ker_off, mask8000 = vis_to_double_dup(0x80008000);

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	gsr_scale = 32 - scalef_expon;
	vis_write_gsr((gsr_scale << 3));

	buf_slb = (2 * dw + 16) >> 3;
	PREPARE_INTERM_BUFFERS();

	dw -= 1;
	dh -= 1;

	sa = adr_src;
	sa1 = sa + slb;
	d_a = adr_dst;

/* load interm. src buff */
#pragma pipeloop(0)
	LOAD_LINE_INTO_BUFFER(sbuf2, sa, 1);

#pragma pipeloop(0)
	for (j = 0; j < dh; j++) {
		LOOP_INI();

#pragma pipeloop(0)
		LOAD_LINE_INTO_BUFFER(sbuf2, sa1, 1);

		vis_alignaddr(s1, 2);
		d1 = *s1;
		d2 = *s2;
		d1 = vis_fxor(d1, mask8000);
		d2 = vis_fxor(d2, mask8000);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 4) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_1 = vis_fxor(d_1, mask8000);
			d_2 = vis_fxor(d_2, mask8000);
			CONV_16_BEGIN(d1, k1);
			d21 = vis_faligndata(d1, d_1);
			CONV_16(d2, k3);
			d22 = vis_faligndata(d2, d_2);
			CONV_16(d21, k2);
			CONV_16(d22, k4);
			(*ddst++) =
			    vis_fxor(vis_fpackfix_pair(out0, out1), mask8000);
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
mlib_v_conv2x2_u16nw_2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon)
{
/* pointers to dst row */
	mlib_u16 *da, *d_a;

/* pointers to src, dst data */
	mlib_u16 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u16 *sa, *sa1;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *prow;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* data */
	mlib_d64 d1, d2, d_1, d_2;
	mlib_f32 k1, k2, k3, k4;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, tmp2, tmp3;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 emask;
	int gsr_scale, i, j;
	mlib_d64 ker_off, mask8000 = vis_to_double_dup(0x80008000);

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	gsr_scale = 32 - scalef_expon;
	vis_write_gsr((gsr_scale << 3));

	buf_slb = (4 * dw + 16) >> 3;
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

		d1 = *s1;
		d2 = *s2;
		d1 = vis_fxor(d1, mask8000);
		d2 = vis_fxor(d2, mask8000);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 4) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_1 = vis_fxor(d_1, mask8000);
			d_2 = vis_fxor(d_2, mask8000);
			CONV_16_BEGIN(d1, k1);
			CONV_16(d2, k3);
			CONV_16_MIX(d1, d_1, k2);
			CONV_16_MIX(d2, d_2, k4);
			(*ddst++) =
			    vis_fxor(vis_fpackfix_pair(out0, out1), mask8000);
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
mlib_v_conv2x2_u16nw_3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon)
{
/* pointers to dst row */
	mlib_u16 *da, *d_a;

/* pointers to src, dst data */
	mlib_u16 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u16 *sa, *sa1;

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
	mlib_f32 k1, k2, k3, k4;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, tmp2, tmp3;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 emask;
	int gsr_scale, i, j;
	mlib_d64 ker_off, mask8000 = vis_to_double_dup(0x80008000);

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	gsr_scale = 32 - scalef_expon;
	vis_write_gsr((gsr_scale << 3));

	buf_slb = (6 * dw + 16) >> 3;
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

		vis_alignaddr(s1, 6);
		d1 = *s1;
		d2 = *s2;
		d1 = vis_fxor(d1, mask8000);
		d2 = vis_fxor(d2, mask8000);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 4) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_1 = vis_fxor(d_1, mask8000);
			d_2 = vis_fxor(d_2, mask8000);
			CONV_16_BEGIN(d1, k1);
			d21 = vis_faligndata(d1, d_1);
			CONV_16(d2, k3);
			d22 = vis_faligndata(d2, d_2);
			CONV_16(d21, k2);
			CONV_16(d22, k4);
			(*ddst++) =
			    vis_fxor(vis_fpackfix_pair(out0, out1), mask8000);
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
mlib_v_conv2x2_u16nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon)
{
/* pointers to dst row */
	mlib_u16 *da, *d_a;

/* pointers to src, dst data */
	mlib_u16 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u16 *sa, *sa1;

/* pointers to rows in interm. src buf */
	mlib_d64 *buff_src, *sbuf1, *sbuf2, *prow;

/* pointer to row in interm. dst buf */
	mlib_d64 *dbuf;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2;

/* mlib_d64 pointer to row in interm. dst buf */
	mlib_d64 *ddst;

/* data */
	mlib_d64 d1, d2, d_1, d_2;
	mlib_f32 k1, k2, k3, k4;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, tmp2, tmp3;
	mlib_d64 *dsa, *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 emask;
	int gsr_scale, i, j;
	mlib_d64 ker_off, mask8000 = vis_to_double_dup(0x80008000);

	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	gsr_scale = 32 - scalef_expon;
	vis_write_gsr((gsr_scale << 3));

	buf_slb = (8 * dw + 16) >> 3;
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

		d1 = *s1;
		d2 = *s2;
		d1 = vis_fxor(d1, mask8000);
		d2 = vis_fxor(d2, mask8000);

#pragma pipeloop(0)
		for (i = 0; i < dw; i += 4) {
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_1 = vis_fxor(d_1, mask8000);
			d_2 = vis_fxor(d_2, mask8000);
			CONV_16_BEGIN(d1, k1);
			CONV_16(d2, k3);
			CONV_16(d_1, k2);
			CONV_16(d_2, k4);
			(*ddst++) =
			    vis_fxor(vis_fpackfix_pair(out0, out1), mask8000);
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
mlib_v_conv2x2_u16nw_mask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
/* pointers to dst row */
	mlib_u16 *da, *d_a;

/* pointers to src, dst data */
	mlib_u16 *adr_dst, *adr_src, *dend;

/* pointers to src rows */
	mlib_u16 *sa, *sa1, *sa2, *sa_2;

/* pointers to rows in interm. src buf */
	mlib_u16 *buff_src, *sbuf1, *sbuf2, *prow;
	mlib_u16 *s_buf1;

/* mlib_d64 pointers to rows in interm. src buf */
	mlib_d64 *s1, *s2;

/* src, dst and interm. buf. strides */
	mlib_s32 dlb, slb, buf_slb;
	mlib_s32 dh, dw;
	mlib_d64 out0, out1, tmp0, tmp1, tmp2, tmp3;

/* data */
	mlib_d64 d1, d2, d_1, d_2;

/* shifted data */
	mlib_d64 d21, d22;

/* coefficients */
	mlib_f32 k1, k2, k3, k4;
	int gsr_scale, i, j, nchannel, chan, testchan;
	mlib_u16 t1, t2, t3, t4, t5, t6, t7, t8;
	type_mlib_d64 str;
	mlib_d64 ker_off, mask8000 = vis_to_double_dup(0x80008000);

	nchannel = mlib_ImageGetChannels(src);
	GET_SRC_DST_PARAMETERS();
	LOAD_KERNEL_INTO_FLOAT();

	gsr_scale = 32 - scalef_expon;
	vis_write_gsr((gsr_scale << 3) + 2);

/* buf_slb - 8-byte aligned */
	buf_slb = (2 * dw + 26) & (~7);
/* alloc. interm. src buffer */
	buff_src =
	    (mlib_u16 *)__mlib_malloc(2 * buf_slb * sizeof (mlib_u8) + 8);

	if (buff_src == NULL)
		return (MLIB_FAILURE);

	buf_slb >>= 1;

	sbuf1 = (mlib_u16 *)((mlib_addr)(buff_src + 8) & (~7));
	sbuf2 = sbuf1 + buf_slb;

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
			da = d_a;
			prow = sbuf1;
			sbuf1 = sbuf2;
			sbuf2 = prow;
			s1 = (mlib_d64 *)sbuf1;
			s2 = (mlib_d64 *)sbuf2;
			dend = da + (dw - 1) * nchannel;
			s_buf1 = sbuf1;
			d1 = *s1;
			d2 = *s2;
			d1 = vis_fxor(d1, mask8000);
			d2 = vis_fxor(d2, mask8000);

			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_1 = vis_fxor(d_1, mask8000);
			d_2 = vis_fxor(d_2, mask8000);
			CONV_16_BEGIN(d1, k1);
			CONV_16(d2, k3);
			d21 = vis_faligndata(d1, d_1);
			d22 = vis_faligndata(d2, d_2);
			CONV_16(d21, k2);
			CONV_16(d22, k4);
			str.value =
			    vis_fxor(vis_fpackfix_pair(out0, out1), mask8000);
			d1 = d_1;
			d2 = d_2;
			s1++;
			s2++;
/*
 * in each iteration store result from prev. iterat.
 * and load data for processing next row
 */
#pragma pipeloop(0)
			for (i = 0; i < dw - 4; i += 4) {
				t1 = *sa_2;
				sa_2 += nchannel;
				t2 = *sa_2;
				sa_2 += nchannel;
				d_1 = *(s1 + 1);
				d_2 = *(s2 + 1);
				d_1 = vis_fxor(d_1, mask8000);
				d_2 = vis_fxor(d_2, mask8000);
				CONV_16_BEGIN(d1, k1);
				t3 = *sa_2;
				sa_2 += nchannel;
				t4 = *sa_2;
				sa_2 += nchannel;
				CONV_16(d2, k3);
				t5 = str.forshort.ushort0;
				t6 = str.forshort.ushort1;
				d21 = vis_faligndata(d1, d_1);
				t7 = str.forshort.ushort2;
				d22 = vis_faligndata(d2, d_2);
				t8 = str.forshort.ushort3;
				CONV_16(d21, k2);
				(*s_buf1++) = t1;
				(*s_buf1++) = t2;
				CONV_16(d22, k4);
				(*s_buf1++) = t3;
				(*s_buf1++) = t4;
				*da = t5;
				da += nchannel;
				str.value =
				    vis_fxor(vis_fpackfix_pair(out0, out1),
				    mask8000);
				*da = t6;
				da += nchannel;
				d1 = d_1;
				d2 = d_2;
				*da = t7;
				da += nchannel;
				s1++;
				s2++;
				*da = t8;
				da += nchannel;
			}

			for (; i < dw + 1; i++) {
				(*s_buf1++) = *sa_2;
				sa_2 += nchannel;
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

			sa_2 = sa2 = sa2 + slb;
			d_a += dlb;
		}

/* process last row - no need to load data */
		da = d_a;
		prow = sbuf1;
		sbuf1 = sbuf2;
		sbuf2 = prow;
		s1 = (mlib_d64 *)sbuf1;
		s2 = (mlib_d64 *)sbuf2;
		dend = da + (dw - 1) * nchannel;
		d1 = *s1;
		d2 = *s2;
		d1 = vis_fxor(d1, mask8000);
		d2 = vis_fxor(d2, mask8000);

		d_1 = *(s1 + 1);
		d_2 = *(s2 + 1);
		d_1 = vis_fxor(d_1, mask8000);
		d_2 = vis_fxor(d_2, mask8000);
		CONV_16_BEGIN(d1, k1);
		CONV_16(d2, k3);
		d21 = vis_faligndata(d1, d_1);
		d22 = vis_faligndata(d2, d_2);
		CONV_16(d21, k2);
		CONV_16(d22, k4);
		d1 = d_1;
		d2 = d_2;
		s1++;
		s2++;

#pragma pipeloop(0)
		for (i = 4; i < dw; i += 4) {
			str.value =
			    vis_fxor(vis_fpackfix_pair(out0, out1), mask8000);
			d_1 = *(s1 + 1);
			d_2 = *(s2 + 1);
			d_1 = vis_fxor(d_1, mask8000);
			d_2 = vis_fxor(d_2, mask8000);
			CONV_16_BEGIN(d1, k1);
			t5 = str.forshort.ushort0;
			CONV_16(d2, k3);
			d21 = vis_faligndata(d1, d_1);
			t6 = str.forshort.ushort1;
			d22 = vis_faligndata(d2, d_2);
			CONV_16(d21, k2);
			t7 = str.forshort.ushort2;
			CONV_16(d22, k4);
			t8 = str.forshort.ushort3;
			*da = t5;
			da += nchannel;
			*da = t6;
			da += nchannel;
			*da = t7;
			da += nchannel;
			d1 = d_1;
			d2 = d_2;
			*da = t8;
			da += nchannel;
			s1++;
			s2++;
		}

		str.value = vis_fxor(vis_fpackfix_pair(out0, out1), mask8000);

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

mlib_status
mlib_conv2x2nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchannel, chan, cmask1, i;
	mlib_status res;

	if (mlib_ImageConvVersion(2, 2, scalef_expon, MLIB_USHORT) == 0)
		return mlib_c_conv2x2nw_u16(dst, src, kernel, scalef_expon,
		    cmask);

	cmask1 = cmask;
	nchannel = mlib_ImageGetChannels(src);
	chan = 0;
	for (i = nchannel - 1; i >= 0; i--, cmask1 >>= 1)
		if ((cmask1 & 1) == 1)
			chan++;

	if ((chan == nchannel)) {
		switch (nchannel) {
		case 1:
			res =
			    mlib_v_conv2x2_u16nw_1(dst, src, kernel,
			    scalef_expon);
			break;
		case 2:
			res =
			    mlib_v_conv2x2_u16nw_2(dst, src, kernel,
			    scalef_expon);
			break;
		case 3:
			res =
			    mlib_v_conv2x2_u16nw_3(dst, src, kernel,
			    scalef_expon);
			break;
		case 4:
			res =
			    mlib_v_conv2x2_u16nw_4(dst, src, kernel,
			    scalef_expon);
			break;
		default:
			res = MLIB_FAILURE;
			break;
		}
	} else
		res =
		    mlib_v_conv2x2_u16nw_mask(dst, src, kernel, scalef_expon,
		    cmask);
	return (res);
}

/* *********************************************************** */
