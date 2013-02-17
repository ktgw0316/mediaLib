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

#pragma ident	"@(#)mlib_v_SignalConv_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_fftConv_S16_S16_Sat
 *      mlib_fftConv_S16S_S16S_Sat
 *
 * DESCRIPTION
 *      Convolution vis FFT call.
 *
 */

#include <vis_proto.h>
#include <mlib_signal.h>
#include <mlib_SignalFFTUtil.h>
#include <mlib_Utils.h>
#include <mlib_SignalConv.h>

/* *********************************************************** */

#if MLIB_VIS == 0x200

#define	FFT_SCALE	1.0/(1 << 15)
#define	GSR_SCALE	16

#else /* MLIB_VIS >= 0x200 */

#define	FFT_SCALE	2.0
#define	GSR_SCALE	0

#endif /* MLIB_VIS >= 0x200 */

/* *********************************************************** */

#define	CONV_S32(DST, SRC)	DST = (mlib_s32)(SRC)

/* *********************************************************** */

#define	MLIB_FRAME	1024

/* *********************************************************** */

static void mlib_v_Conv_2short2double(
    mlib_d64 *pdst,
    mlib_s16 *psrc,
    mlib_s32 n);

static void mlib_v_Conv_2shortS2double(
    mlib_d64 *pdst1,
    mlib_d64 *pdst2,
    mlib_s16 *psrc,
    mlib_s32 n);

static void mlib_v_Conv_double2short(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n);

static void mlib_v_Conv_doubleS2short(
    mlib_s16 *pdst,
    mlib_d64 *psrc1,
    mlib_d64 *psrc2,
    mlib_s32 n);

/* *********************************************************** */

mlib_status
mlib_fftConv_S16_S16_Sat(
    mlib_s16 *dst,
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 z;
	mlib_s32 i, s;
	mlib_s32 k, order;
	mlib_d64 a_src1[MLIB_FRAME * 4];
	mlib_d64 *psrc1 = a_src1, *pdst1;

/* fft way */

	switch (m) {
	case 0:
		return (MLIB_SUCCESS);

	case 1:
		z = (mlib_d64)src1[0] * 2.;

		for (i = 0; i < n; i++) {
			SAT_S16(dst[i], z * src2[i]);
		}

		return (MLIB_SUCCESS);

	case 2: {
		    mlib_d64 mult0 = src1[0] * 2., mult1 = src1[1] * 2.;

		    SAT_S16(dst[0], mult0 * src2[0]);

		    for (i = 1; i < n; i++) {
			    SAT_S16(dst[i],
				mult0 * src2[i] + mult1 * src2[i - 1]);
		    }

		    SAT_S16(dst[n], mult1 * src2[n - 1]);
	    }

		return (MLIB_SUCCESS);
	}

	s = n + m - 1;

	for (order = 0, k = 1; k < s; ) {
		order++;
		k <<= 1;
	}

	if (k > MLIB_FRAME / 2) {
		if (!(psrc1 = __mlib_malloc(2 * 4 * k * sizeof (mlib_d64))))
			return (MLIB_FAILURE);
	}

	pdst1 = psrc1 + 4 * k;

	mlib_v_Conv_2short2double(psrc1, src1, m);

#pragma pipeloop(0)
	for (i = m; i < n; i++) {
		psrc1[2 * i] = 0.;
	}

	mlib_v_Conv_2short2double(psrc1 + 1, src2, n);

#pragma pipeloop(0)
	for (i = n; i < k; i++) {
		psrc1[2 * i] = psrc1[2 * i + 1] = 0.;
	}

#pragma pipeloop(0)
	for (i = n; i < k; i++) {
		psrc1[2 * i] = psrc1[2 * i + 1] = 0.;
	}

	mlib_conv_fft(pdst1, psrc1, order, FFT_SCALE);

	mlib_v_Conv_double2short(dst, pdst1, s);

	if (k > MLIB_FRAME / 2)
		__mlib_free(psrc1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_fftConv_S16S_S16S_Sat(
    mlib_s16 *dst,
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 z, z1;
	mlib_s32 i, s;
	mlib_s32 k, order;
	mlib_d64 a_src1[MLIB_FRAME * 4 * 2 + 63];

/* 63 added in help to trick direct-mapped cache :) */
	mlib_d64 *psrc1 = a_src1, *pdst1;
	mlib_d64 *psrc2, *pdst2;

/* fft way */
	switch (m) {
	case 0:
		return (MLIB_SUCCESS);

	case 1:
		z = (mlib_d64)src1[0] * 2.;
		z1 = (mlib_d64)src1[1] * 2.;
		for (i = 0; i < n; i++) {
			SAT_S16(dst[2 * i], z * src2[2 * i]);
			SAT_S16(dst[2 * i + 1], z1 * src2[2 * i + 1]);
		}

		return (MLIB_SUCCESS);

	case 2: {
		    mlib_d64 mult0 = src1[0] * 2., mult1 = src1[2] * 2.;
		    mlib_d64 mult01 = src1[1] * 2., mult11 = src1[3] * 2.;

		    SAT_S16(dst[0], mult0 * src2[0]);
		    SAT_S16(dst[1], mult01 * src2[1]);
		    for (i = 1; i < n; i++) {
			    SAT_S16(dst[2 * i],
				mult0 * src2[2 * i] + mult1 * src2[2 * (i -
				1)]);
			    SAT_S16(dst[2 * i + 1],
				mult01 * src2[2 * i + 1] +
				mult11 * src2[2 * (i - 1) + 1]);
		    }

		    SAT_S16(dst[2 * n], mult1 * src2[2 * (n - 1)]);
		    SAT_S16(dst[2 * n + 1], mult11 * src2[2 * (n - 1) + 1]);
	    }

		return (MLIB_SUCCESS);
	}

	s = n + m - 1;
	for (order = 0, k = 1; k < s; ) {
		order++;
		k <<= 1;
	}

	if (k > MLIB_FRAME / 2) {
		if (!(psrc1 =
		    __mlib_malloc((2 * 2 * 4 * k + 63) * sizeof (mlib_d64))))
			return (MLIB_FAILURE);
	}

	pdst1 = psrc1 + 4 * k;
	psrc2 = pdst1 + 4 * k + 63;
	pdst2 = psrc2 + 4 * k;

	mlib_v_Conv_2shortS2double(psrc1, psrc2, src1, m);

#pragma pipeloop(0)
	for (i = m; i < n; i++) {
		psrc1[2 * i] = psrc2[2 * i] = 0.;
	}

	mlib_v_Conv_2shortS2double(psrc1 + 1, psrc2 + 1, src2, n);

#pragma pipeloop(0)
	for (i = n; i < k; i++) {
		psrc1[2 * i] = psrc1[2 * i + 1] = psrc2[2 * i] =
		    psrc2[2 * i + 1] = 0.;
	}

	mlib_conv_fft(pdst1, psrc1, order, FFT_SCALE);

	mlib_conv_fft(pdst2, psrc2, order, FFT_SCALE);

	mlib_v_Conv_doubleS2short(dst, pdst1, pdst2, s);

	if (k > MLIB_FRAME / 2)
		__mlib_free(psrc1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static void
mlib_v_Conv_2short2double(
    mlib_d64 *pdst,
    mlib_s16 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 data;
	mlib_f32 fone = vis_to_float(0x00010001);
	type_union_mlib_d64 str1, str2;

	for (i = 0; i < n && (7 & (mlib_addr)psrc) != 0; i++) {
		(*pdst++) = (mlib_d64)(*psrc++);
		(*pdst++);
	}

#pragma pipeloop(0)
	for (; i + 4 < n; i += 4) {
		data = *(mlib_d64 *)psrc;

		str1.value = vis_fmuld8ulx16(fone, vis_read_hi(data));
		str2.value = vis_fmuld8ulx16(fone, vis_read_lo(data));

		pdst[0] = (mlib_d64)str1.twoint.int0;
		pdst[2] = (mlib_d64)str1.twoint.int1;
		pdst[4] = (mlib_d64)str2.twoint.int0;
		pdst[6] = (mlib_d64)str2.twoint.int1;

		psrc += 4;
		pdst += 8;
	}

	for (; i < n; i++) {
		(*pdst++) = (mlib_d64)(*psrc++);
		(*pdst++);
	}
}

/* *********************************************************** */

static void
mlib_v_Conv_2shortS2double(
    mlib_d64 *pdst1,
    mlib_d64 *pdst2,
    mlib_s16 *psrc,
    mlib_s32 n)
{
	mlib_s32 i, oddeven, stopit = ((7 & (mlib_addr)psrc) == 0);
	mlib_d64 data;
	mlib_d64 *pd1 = pdst1, *pd2 = pdst2;
	mlib_s16 *ps = psrc;
	mlib_f32 fone = vis_to_float(0x00010001);
	type_union_mlib_d64 str1, str2;

	for (i = 0; i < n && !stopit; i++) {
		(*pdst1++) = (mlib_d64)(*psrc++);
		pdst1++;
		stopit |= ((7 & (mlib_addr)psrc) == 0);
		(*pdst2++) = (mlib_d64)(*psrc++);
		pdst2++;
		stopit |= ((7 & (mlib_addr)psrc) == 0);
	}

	oddeven = 7 & (mlib_addr)psrc;

	if (oddeven) {
		pd1 = pdst2 - 2;
		pd2 = pdst1;
		ps = psrc - 1;
	} else {
		pd1 = pdst1;
		pd2 = pdst2;
		ps = psrc;
	}

#pragma pipeloop(0)
	for (; i < (n - 1); i += 2) {
		data = *(mlib_d64 *)ps;

		str1.value = vis_fmuld8ulx16(fone, vis_read_hi(data));
		str2.value = vis_fmuld8ulx16(fone, vis_read_lo(data));

		(*pd1++) = (mlib_d64)str1.twoint.int0;
		pd1++;
		(*pd2++) = (mlib_d64)str1.twoint.int1;
		pd2++;
		(*pd1++) = (mlib_d64)str2.twoint.int0;
		pd1++;
		(*pd2++) = (mlib_d64)str2.twoint.int1;
		pd2++;

		ps += 4;
	}

	if (oddeven) {
		pdst1 = pd2;
		pdst2 = pd1;
		psrc = ps;
		(*pdst2++) = (mlib_d64)(*psrc++);
		pdst2++;
	} else {
		pdst1 = pd1;
		pdst2 = pd2;
		psrc = ps;
	}

	if (i < n) {
		(*pdst1++) = (mlib_d64)(*psrc++);
		pdst1++;
		(*pdst2++) = (mlib_d64)(*psrc++);
		pdst2++;
	}
}

/* *********************************************************** */

static void
mlib_v_Conv_double2short(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 data;
	type_union_mlib_d64 str1, str2;

	vis_write_gsr(GSR_SCALE << 3);

	for (i = 0; i < n && (7 & (mlib_addr)pdst) != 0; i++) {
		data = (*psrc++);
		CLAMP_S16((*pdst++), data);
	}

#pragma pipeloop(0)
	for (; i < (n - 4); i += 4) {

		CONV_S32(str1.twoint.int0, psrc[0]);
		CONV_S32(str1.twoint.int1, psrc[1]);
		CONV_S32(str2.twoint.int0, psrc[2]);
		CONV_S32(str2.twoint.int1, psrc[3]);

		data = vis_fpackfix_pair(str1.value, str2.value);

		*(mlib_d64 *)pdst = data;

		psrc += 4;
		pdst += 4;
	}

	for (; i < n; i++) {
		data = (*psrc++);
		CLAMP_S16((*pdst++), data);
	}
}

/* *********************************************************** */

static void
mlib_v_Conv_doubleS2short(
    mlib_s16 *pdst,
    mlib_d64 *psrc1,
    mlib_d64 *psrc2,
    mlib_s32 n)
{
	mlib_s32 i, oddeven, stopit = ((7 & (mlib_addr)pdst) == 0);
	mlib_d64 data1, data2, *p1 = psrc1, *p2 = psrc2;
	mlib_s16 *pd = pdst;
	type_union_mlib_d64 str1, str2;
	mlib_s32 nn = n * 2;

	vis_write_gsr(GSR_SCALE << 3);

	for (i = 0; i < (nn - 1) && !stopit; i += 2) {
		data1 = (*psrc1++);
		CLAMP_S16((*pdst++), data1);
		stopit |= ((7 & (mlib_addr)pdst) == 0);
		data2 = (*psrc2++);
		CLAMP_S16((*pdst++), data2);
		stopit |= ((7 & (mlib_addr)pdst) == 0);
	}

	oddeven = 7 & (mlib_addr)pdst;

	if (oddeven) {
		p1 = psrc2 - 1;
		p2 = psrc1;
		pd = pdst - 1;
		i--;
	} else {
		p1 = psrc1;
		p2 = psrc2;
		pd = pdst;
	}

#pragma pipeloop(0)
	for (; i + 4 < nn; i += 4) {

		CONV_S32(str1.twoint.int0, p1[0]);
		CONV_S32(str1.twoint.int1, p2[0]);
		CONV_S32(str2.twoint.int0, p1[1]);
		CONV_S32(str2.twoint.int1, p2[1]);

		data1 = vis_fpackfix_pair(str1.value, str2.value);

		*(mlib_d64 *)pd = data1;

		p1 += 2;
		p2 += 2;
		pd += 4;
	}

	if (oddeven) {
		psrc2 = p1;
		psrc1 = p2;
		pdst = pd;
		data2 = (*psrc2++);
		CLAMP_S16((*pdst++), data2);
		i++;
	} else {
		psrc1 = p1;
		psrc2 = p2;
		pdst = pd;
	}

	for (; i < (nn - 1); i += 2) {
		data1 = (*psrc1++);
		CLAMP_S16((*pdst++), data1);
		data2 = (*psrc2++);
		CLAMP_S16((*pdst++), data2);
	}
}

/* *********************************************************** */
