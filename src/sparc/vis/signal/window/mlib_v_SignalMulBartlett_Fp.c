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

#pragma ident	"@(#)mlib_v_SignalMulBartlett_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulBartlett_F32 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulBartlett_F32S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulBartlett_F32_F32 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulBartlett_F32S_F32S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulBartlett_F32(mlib_f32 *data,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalMulBartlett_F32S(mlib_f32 *data,
 *                                              mlib_s32 n);
 *      mlib_status mlib_SignalMulBartlett_F32S_F32S(mlib_f32       *dst,
 *                                                   const mlib_f32 *src,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulBartlett_F32_F32(mlib_f32       *dst,
 *                                                 const mlib_f32 *src,
 *                                                 mlib_s32       n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Length of window array
 *
 * DESCRIPTION
 *
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *
 *      Bartlett    :   window[i] = ((i <= (N - 1)/2) ? (2*i/(N -1)) :
 *                        (2 - (2 * i)/(N-1)))
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <vis_proto.h>
#include <mlib_SignalWindowUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulBartlett_F32 = __mlib_SignalMulBartlett_F32
#pragma weak mlib_SignalMulBartlett_F32S = __mlib_SignalMulBartlett_F32S
#pragma weak mlib_SignalMulBartlett_F32S_F32S = \
	__mlib_SignalMulBartlett_F32S_F32S
#pragma weak mlib_SignalMulBartlett_F32_F32 = \
	__mlib_SignalMulBartlett_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulBartlett_F32) mlib_SignalMulBartlett_F32
    __attribute__((weak, alias("__mlib_SignalMulBartlett_F32")));
__typeof__(__mlib_SignalMulBartlett_F32S) mlib_SignalMulBartlett_F32S
    __attribute__((weak, alias("__mlib_SignalMulBartlett_F32S")));
__typeof__(__mlib_SignalMulBartlett_F32S_F32S) mlib_SignalMulBartlett_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalMulBartlett_F32S_F32S")));
__typeof__(__mlib_SignalMulBartlett_F32_F32) mlib_SignalMulBartlett_F32_F32
    __attribute__((weak, alias("__mlib_SignalMulBartlett_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 i0, i1;
	} f32x2;
} mlib_union64;

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_F32S(
    mlib_f32 *data,
    mlib_s32 n)
{
	mlib_union64 tmp_1, tmp_2;
	mlib_d64 step, base, tmp1, tmp2;
	mlib_d64 tmp1_new, tmp2_new, tmp1_last, tmp2_last;
	mlib_s32 n1, i;
	mlib_f32 *data2 = data + 2 * n - 1;
	mlib_d64 *pdata, *pdata2;
	mlib_u8 emask1, emask2;

	if ((data == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	n1 = n - 1;
	step = 2. / n1;
	base = 0.;

	if ((mlib_addr)data & 7) {

		pdata = vis_alignaddr((void *)data, 0);
		pdata2 = (mlib_d64 *)data2;
		emask1 = 3;
		emask2 = 12;

		tmp1_last = vis_to_double_dup(0);
		data += 2;
		tmp2_last = vis_to_double_dup(0);
		data2 -= 2;

		vis_pst_16(tmp1_last, pdata, emask1);
		vis_pst_16(tmp2_last, pdata2, emask2);

		pdata++;
		pdata2--;
		base += step;
		n = n - 1;

#pragma pipeloop(0)
		for (i = 0; i < n / 2; i++) {

			tmp_1.f32x2.i0 = base * data[0];
			tmp_1.f32x2.i1 = base * data[1];
			tmp1 = tmp_1.d64;
			data += 2;

			tmp_2.f32x2.i1 = base * data2[0];
			data2--;
			tmp_2.f32x2.i0 = base * data2[0];
			tmp2 = tmp_2.d64;
			data2--;

			tmp1_new = vis_faligndata(tmp1_last, tmp1);
			tmp1_last = tmp1;

			tmp2_new = vis_faligndata(tmp2, tmp2_last);
			tmp2_last = tmp2;

			pdata[0] = tmp1_new;
			pdata++;
			pdata2[0] = tmp2_new;
			pdata2--;

			base += step;
		}

		if (n & 1) {
			data--;
			data[0] = data[0] * (base - step);
			data[1] = data[1] * (base - step);
		}
	} else {

		pdata = (mlib_d64 *)data;
		pdata2 = (mlib_d64 *)(data2 - 1);

#pragma pipeloop(0)
		for (i = 0; i < n / 2; i++) {

			tmp_1.f32x2.i0 = base * data[0];
			tmp_1.f32x2.i1 = base * data[1];
			tmp1 = tmp_1.d64;

			tmp_2.f32x2.i1 = base * data2[0];
			data2--;
			tmp_2.f32x2.i0 = base * data2[0];
			tmp2 = tmp_2.d64;

			pdata[0] = tmp1;
			pdata++;
			pdata2[0] = tmp2;
			pdata2--;
			data2--;
			data += 2;

			base += step;
		}

		if (n & 1) {
			data--;
			data[0] = data[0] * base;
			data[1] = data[1] * base;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n)
{
	mlib_union64 tmp_1, tmp_2;
	mlib_s32 n1, i, k = 0;
	mlib_d64 tmp1, base, cbase0, cbase1, step, *pdst, *pdst2, *psrc, *psrc2;
	mlib_d64 tmp2, dat1, dat2;
	mlib_f32 *dst2 = dst + n - 1;
	const mlib_f32 *src2 = src + n - 1;

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	n1 = n - 1;
	step = 2. / n1;
	cbase0 = 0.;
	cbase1 = cbase0 + step;
	base = cbase0;

	if ((mlib_addr)dst & 7) {
		(*dst++) = 0.;
		src++;
		*dst2-- = 0.;
		src2--;
		n = n - 2;
		cbase0 = cbase1;
		cbase1 = cbase0 + step;
		base = base + step;
	}

	if (n == 0)
		return (MLIB_SUCCESS);

	if (n == 1) {
		*dst = src[0] * cbase0;
		return (MLIB_SUCCESS);
	}

	pdst = (mlib_d64 *)(dst);

	if (((mlib_addr)(dst2 - 1) & 7) && (!((mlib_addr)src & 7))) {

		if (n > 3) {

			pdst2 = (mlib_d64 *)(dst2 - 2);
			psrc = (mlib_d64 *)(src);
			psrc2 = (mlib_d64 *)(src2 - 2);

			tmp_1.f32x2.i0 = vis_read_hi(psrc[0]) * cbase0;
			tmp_1.f32x2.i1 = vis_read_lo(psrc[0]) * cbase1;
			tmp1 = tmp_1.d64;

			dst2[0] = src2[0] * base;

			*pdst = tmp1;
			pdst++;
			psrc++;

			cbase0 = cbase1 + step;
			cbase1 = cbase0 + step;
			base += step;

			n = n - 3;
			k = k + 2;

#pragma pipeloop(0)
			for (i = 0; i < n / 4; i++) {

				tmp_1.f32x2.i0 = vis_read_hi(psrc[0]) * cbase0;
				tmp_1.f32x2.i1 = vis_read_lo(psrc[0]) * cbase1;
				tmp1 = tmp_1.d64;

				tmp_2.f32x2.i1 = vis_read_lo(psrc2[0]) * base;
				base += step;
				tmp_2.f32x2.i0 = vis_read_hi(psrc2[0]) * base;
				tmp2 = tmp_2.d64;

				*pdst = tmp1;
				pdst++;
				psrc++;
				*pdst2 = tmp2;
				pdst2--;
				psrc2--;

				cbase0 = cbase1 + step;
				cbase1 = cbase0 + step;
				base += step;
			}

			if (n & 3) {
				dst += 2 * i + k;
				src += 2 * i + k;
				dst[1] = (cbase0 - step) * src[1];
				dst[0] = (cbase1 - step) * src[0];
			}
		} else {
			dst[2] = cbase0 * src[2];
			dst[1] = cbase1 * src[1];
			dst[0] = cbase0 * src[0];
		}
	}

	if (((mlib_addr)(dst2 - 1) & 7) && ((mlib_addr)src & 7)) {

		if (n > 3) {

			pdst2 = (mlib_d64 *)(dst2 - 2);

			tmp_1.f32x2.i0 = src[0] * cbase0;
			tmp_1.f32x2.i1 = src[1] * cbase1;
			tmp1 = tmp_1.d64;

			dst2[0] = src2[0] * base;

			*pdst = tmp1;
			pdst++;
			src += 2;
			src2--;

			cbase0 = cbase1 + step;
			cbase1 = cbase0 + step;
			base += step;

			n = n - 3;
			k = k + 2;

#pragma pipeloop(0)
			for (i = 0; i < n / 4; i++) {

				tmp_1.f32x2.i0 = src[0] * cbase0;
				tmp_1.f32x2.i1 = src[1] * cbase1;
				tmp1 = tmp_1.d64;

				tmp_2.f32x2.i1 = src2[0] * base;
				base += step;
				src2--;
				tmp_2.f32x2.i0 = src2[0] * base;
				tmp2 = tmp_2.d64;

				*pdst = tmp1;
				pdst++;
				src += 2;
				*pdst2 = tmp2;
				pdst2--;
				src2--;

				cbase0 = cbase1 + step;
				cbase1 = cbase0 + step;
				base += step;
			}

			if (n & 3) {
				dst += 2 * i + k;
				dst[1] = (cbase0 - step) * src[1];
				dst[0] = (cbase1 - step) * src[0];
			}
		} else {
			dst[2] = cbase0 * src[2];
			dst[1] = cbase1 * src[1];
			dst[0] = cbase0 * src[0];
		}
	}

	if ((!((mlib_addr)(dst2 - 1) & 7)) && ((mlib_addr)src & 7)) {

		if (n > 3) {

			pdst2 = (mlib_d64 *)(dst2 - 1);

#pragma pipeloop(0)
			for (i = 0; i < n / 4; i++) {

				tmp_1.f32x2.i0 = src[0] * cbase0;
				tmp_1.f32x2.i1 = src[1] * cbase1;
				tmp1 = tmp_1.d64;

				tmp_2.f32x2.i1 = src2[0] * cbase0;
				src2--;
				tmp_2.f32x2.i0 = src2[0] * cbase1;
				tmp2 = tmp_2.d64;

				*pdst = tmp1;
				pdst++;
				src += 2;
				*pdst2 = tmp2;
				pdst2--;
				src2--;

				cbase0 = cbase1 + step;
				cbase1 = cbase0 + step;
			}

			if (n & 3) {
				dst += 2 * i;
				dst[1] = cbase0 * src[1];
				dst[0] = cbase0 * src[0];
			}
		} else {
			dst[1] = cbase0 * src[1];
			dst[0] = cbase0 * src[0];
		}
	}

	if ((!((mlib_addr)(dst2 - 1) & 7)) && (!((mlib_addr)src & 7))) {

		if (n > 3) {

			pdst2 = (mlib_d64 *)(dst2 - 1);
			psrc2 = (mlib_d64 *)(src2 - 1);
			psrc = (mlib_d64 *)src;
#pragma pipeloop(0)
			for (i = 0; i < n / 4; i++) {

				dat1 = *psrc;
				tmp_1.f32x2.i0 = cbase0 * vis_read_hi(dat1);
				tmp_1.f32x2.i1 = cbase1 * vis_read_lo(dat1);
				tmp1 = tmp_1.d64;

				dat2 = *psrc2;
				tmp_2.f32x2.i1 = cbase0 * vis_read_lo(dat2);
				tmp_2.f32x2.i0 = cbase1 * vis_read_hi(dat2);
				tmp2 = tmp_2.d64;

				*pdst = tmp1;
				pdst++;
				psrc++;
				*pdst2 = tmp2;
				pdst2--;
				psrc2--;

				cbase0 = cbase1 + step;
				cbase1 = cbase0 + step;
			}

			if (n & 3) {
				dst += 2 * i;
				src += 2 * i;
				dst[1] = cbase0 * src[1];
				dst[0] = cbase0 * src[0];
			}
		} else {
			dst[1] = cbase0 * src[1];
			dst[0] = cbase0 * src[0];
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n)
{
	mlib_union64 tmp_1, tmp_2;
	mlib_d64 step, base, tmp1, tmp2;
	mlib_d64 tmp1_new, tmp2_new, tmp1_last, tmp2_last;
	mlib_s32 n1, i;
	mlib_f32 *dst2 = dst + 2 * n - 1;
	const mlib_f32 *src2 = src + 2 * n - 1;
	mlib_d64 *pdst, *pdst2;
	mlib_u8 emask1, emask2;

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	n1 = n - 1;
	step = 2. / n1;
	base = 0.;

	if ((mlib_addr)dst & 7) {

		pdst = vis_alignaddr((void *)dst, 0);
		pdst2 = (mlib_d64 *)dst2;
		emask1 = 3;
		emask2 = 12;

		tmp1_last = vis_to_double_dup(0);
		src += 2;
		tmp2_last = vis_to_double_dup(0);
		src2 -= 2;

		vis_pst_16(tmp1_last, pdst, emask1);
		vis_pst_16(tmp2_last, pdst2, emask2);

		pdst++;
		pdst2--;
		base += step;
		n = n - 1;

#pragma pipeloop(0)
		for (i = 0; i < n / 2; i++) {

			tmp_1.f32x2.i0 = base * src[0];
			tmp_1.f32x2.i1 = base * src[1];
			tmp1 = tmp_1.d64;
			src += 2;

			tmp_2.f32x2.i1 = base * src2[0];
			src2--;
			tmp_2.f32x2.i0 = base * src2[0];
			tmp2 = tmp_2.d64;
			src2--;

			tmp1_new = vis_faligndata(tmp1_last, tmp1);
			tmp1_last = tmp1;

			tmp2_new = vis_faligndata(tmp2, tmp2_last);
			tmp2_last = tmp2;

			pdst[0] = tmp1_new;
			pdst++;
			pdst2[0] = tmp2_new;
			pdst2--;

			base += step;
		}

		if (n & 1) {
			dst += i * 2 + 1;
			src--;
			dst[0] = src[0] * (base - step);
			dst[1] = src[1] * (base - step);
		}
	} else {

		pdst = (mlib_d64 *)dst;
		pdst2 = (mlib_d64 *)(dst2 - 1);

#pragma pipeloop(0)
		for (i = 0; i < n / 2; i++) {

			tmp_1.f32x2.i0 = base * src[0];
			tmp_1.f32x2.i1 = base * src[1];
			tmp1 = tmp_1.d64;

			tmp_2.f32x2.i1 = base * src2[0];
			src2--;
			tmp_2.f32x2.i0 = base * src2[0];
			tmp2 = tmp_2.d64;

			pdst[0] = tmp1;
			pdst++;
			pdst2[0] = tmp2;
			pdst2--;
			src2--;
			src += 2;

			base += step;
		}

		if (n & 1) {
			dst += i * 2;
			dst[0] = src[0] * base;
			dst[1] = src[1] * base;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_F32(
    mlib_f32 *data,
    mlib_s32 n)
{
	mlib_union64 tmp_1, tmp_2;
	mlib_s32 n1, i, k = 0;
	mlib_d64 tmp1, cbase0, cbase1, step, *pdata, *pdata2;
	mlib_d64 tmp2, dat1, dat2, base;
	mlib_f32 *data2 = data + n - 1;

	if ((data == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	n1 = n - 1;
	step = 2. / n1;
	cbase0 = 0.;
	cbase1 = cbase0 + step;

	if ((mlib_addr)data & 7) {
		(*data++) = 0.;
		*data2-- = 0.;
		n = n - 2;
		cbase0 = cbase1;
		cbase1 = cbase0 + step;
	}

	if (n == 1) {
		*data = (*data) * cbase0;
		return (MLIB_SUCCESS);
	}

	pdata = (mlib_d64 *)(data);

	if (!((mlib_addr)(data2 - 1) & 7)) {
		pdata2 = (mlib_d64 *)(data2 - 1);
#pragma pipeloop(0)
		for (i = 0; i < n / 4; i++) {
			dat1 = *pdata;
			tmp_1.f32x2.i0 = cbase0 * vis_read_hi(dat1);
			tmp_1.f32x2.i1 = cbase1 * vis_read_lo(dat1);
			tmp1 = tmp_1.d64;

			dat2 = *pdata2;
			tmp_2.f32x2.i1 = cbase0 * vis_read_lo(dat2);
			tmp_2.f32x2.i0 = cbase1 * vis_read_hi(dat2);
			tmp2 = tmp_2.d64;

			*pdata = tmp1;
			pdata++;
			*pdata2 = tmp2;
			pdata2--;

			cbase0 = cbase1 + step;
			cbase1 = cbase0 + step;
		}

		if (n & 3) {
			data += 2 * i;
			data[1] = data[1] * cbase0;
			data[0] = data[0] * cbase0;
		}
	} else {

		if (n > 3) {

			pdata2 = (mlib_d64 *)(data2 - 2);

			dat1 = pdata[0];
			tmp_1.f32x2.i0 = cbase0 * vis_read_hi(dat1);
			tmp_1.f32x2.i1 = cbase1 * vis_read_lo(dat1);
			tmp1 = tmp_1.d64;

			pdata[0] = tmp1;
			pdata++;

			base = cbase0;
			data2[0] = data2[0] * base;

			cbase0 = cbase1 + step;
			cbase1 = cbase0 + step;
			base += step;

			n = n - 3;
			k = k + 2;

#pragma pipeloop(0)
			for (i = 0; i < n / 4; i++) {
				dat1 = pdata[0];
				tmp_1.f32x2.i0 = cbase0 * vis_read_hi(dat1);
				tmp_1.f32x2.i1 = cbase1 * vis_read_lo(dat1);
				tmp1 = tmp_1.d64;

				pdata[0] = tmp1;
				pdata++;

				dat2 = *pdata2;
				tmp_2.f32x2.i1 = base * vis_read_lo(dat2);
				base += step;
				tmp_2.f32x2.i0 = base * vis_read_hi(dat2);
				tmp2 = tmp_2.d64;

				pdata2[0] = tmp2;
				pdata2--;

				cbase0 = cbase1 + step;
				cbase1 = cbase0 + step;
				base += step;
			}

			if (n & 3) {
				data += 2 * i + k;
				data[1] = (cbase0 - step) * data[1];
				data[0] = (cbase1 - step) * data[0];
			}
		} else {
			data[2] = cbase0 * data[2];
			data[1] = cbase1 * data[1];
			data[0] = cbase0 * data[0];
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
