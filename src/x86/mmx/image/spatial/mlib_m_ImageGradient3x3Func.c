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

#pragma ident	"@(#)mlib_m_ImageGradient3x3Func.c	9.2	07/11/05 SMI"

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */
extern mlib_u8 mlib_SqrtTable_U8[];
extern mlib_u16 mlib_SqrtTable_U8_Diap[];

/* *********************************************************** */
typedef struct mlib_grad_ws
{
	__m64 *kern;
	__m64 *buff0;
	__m64 *buff1;
	__m64 *buff2;
	mlib_s32 width;
	mlib_s32 wid4;
} mlib_grad_ws;

/* *********************************************************** */
typedef union
{
	__m64 dv;
	mlib_u16 sv[4];
} mlib_union_type;

/* *********************************************************** */
#define	FAST_SQRT_U8(dp, v)                                     \
	{                                                       \
	    mlib_s32 diap;                                      \
	                                                        \
	    diap = mlib_SqrtTable_U8_Diap[(v) >> 8];            \
	    *(dp) = mlib_SqrtTable_U8[((v) >> diap) + diap];    \
	}

/* *********************************************************** */
#define	ALGN(x, y, sh)                                                        \
		_mm_or_si64(_mm_srli_si64(x, sh), _mm_slli_si64(y, 64 - sh))

/* *********************************************************** */
#define	GET_SRC_DST_PARAMETERS(type)                               \
	mlib_s32 height = mlib_ImageGetHeight(src);                \
	mlib_s32 width = mlib_ImageGetWidth(src);                  \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type);   \
	    mlib_s32                                               \
		dll = mlib_ImageGetStride(dst) / sizeof (type);    \
	    type *                                                 \
		sl = (type *) mlib_ImageGetData(src);              \
	    type *                                                 \
		dl = (type *) mlib_ImageGetData(dst)

/* *********************************************************** */
#define	GET_KERN()                                              \
	ker1 = (ws->kern)[0];                                   \
	ker2 = (ws->kern)[1];                                   \
	ker3 = (ws->kern)[2];                                   \
	ker4 = (ws->kern)[3];                                   \
	ker5 = (ws->kern)[4];                                   \
	ker6 = (ws->kern)[5];                                   \
	ker7 = (ws->kern)[6];                                   \
	ker8 = (ws->kern)[7];                                   \
	ker9 = (ws->kern)[8]

/* *********************************************************** */
#define	PREP_3x3()                                              \
	d0 = d1;                                                \
	UNPACK_SRC(d1, lo);                                     \
	sum0 = _mm_mulhi_pi16(d0, ker1);                        \
	sum1 = _mm_mulhi_pi16(d0, ker4);                        \
	aa = ALGN(d0, d1, 16);                                  \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(aa, ker2));    \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(aa, ker5));    \
	bb = ALGN(d0, d1, 32);                                  \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(bb, ker3));    \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(bb, ker6));    \
	buff1[i] = sum0;                                        \
	buff2[i] = sum1

/* *********************************************************** */
#define	CONV_3x3(xx, i)                                         \
	d0 = d1;                                                \
	UNPACK_SRC(d1, xx);                                     \
	prev0 = buff0[i];                                       \
	prev1 = buff2[i];                                       \
	sum0 = _mm_mulhi_pi16(d0, ker1);                        \
	sum1 = _mm_mulhi_pi16(d0, ker4);                        \
	sum2 = _mm_mulhi_pi16(d0, ker7);                        \
	aa = ALGN(d0, d1, 16);                                  \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(aa, ker2));    \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(aa, ker5));    \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(aa, ker8));    \
	bb = ALGN(d0, d1, 32);                                  \
	sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(bb, ker3));    \
	sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(bb, ker6));    \
	sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(bb, ker9));    \
	sum2 = _mm_add_pi16(sum2, prev0);                       \
	sum2 = _mm_add_pi16(sum2, prev1);                       \
	buff0[i] = sum0;                                        \
	buff2[i] = sum1

/* *********************************************************** */
#define	UNPACK_SRC(dd, xx)                                      \
	dd = _mm_unpack##xx##_pi8(s0, zero);                    \
	dd = _mm_slli_pi16(d1, 7)

/* *********************************************************** */
void
mlib_m_grad3x3_prep(
    mlib_grad_ws * ws,
    mlib_s32 j,
    mlib_s32 *sp)
{
	__m64 *buff0, *buff1, *buff2;
	__m64 ker1, ker2, ker3, ker4, ker5, ker6, ker7, ker8, ker9;
	__m64 s0, d0, d1, sum0, sum1, aa, bb;
	__m64 zero = _m_zero;
	mlib_s32 wid4 = ws->wid4;
	mlib_s32 i;

	buff0 = ws->buff0;
	buff1 = ws->buff1;
	buff2 = ws->buff2;

	GET_KERN();

	*(mlib_s32 *)&s0 = (*sp++);
	UNPACK_SRC(d1, lo);

	for (i = 0; i < wid4; i++) {
		*(mlib_s32 *)&s0 = sp[i];

		PREP_3x3();
	}

	if (j == 0) {
		ws->buff1 = buff0;
		ws->buff0 = buff1;
	}

	_mm_empty();
}

/* *********************************************************** */
void
mlib_m_grad3x3_line(
    mlib_grad_ws * ws,
    __m64 * dp,
    __m64 * sp)
{
	__m64 *buff0, *buff1, *buff2;
	__m64 ker1, ker2, ker3, ker4, ker5, ker6, ker7, ker8, ker9;
	__m64 s0, d0, d1, prev0, prev1, sum0, sum1, sum2, aa, bb;
	__m64 zero = _m_zero;
	mlib_s32 wid4 = ws->wid4;
	mlib_s32 i;

	buff0 = ws->buff0;
	buff1 = ws->buff1;
	buff2 = ws->buff2;

	GET_KERN();

	s0 = (*sp++);
	UNPACK_SRC(d1, lo);

	for (i = 0; i < wid4 / 2; i++) {
		CONV_3x3(hi, 2 * i);
		dp[2 * i] = sum2;

		s0 = sp[i];
		CONV_3x3(lo, 2 * i + 1);
		dp[2 * i + 1] = sum2;
	}

	ws->buff1 = buff0;
	ws->buff0 = buff1;

	_mm_empty();
}

/* *********************************************************** */
static void
mlib_m_ImageGradient3x3U82D64(
    __m64 * dst,
    mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_u8 *dst8 = (mlib_u8 *)dst;
	mlib_s32 i;

/*
 * !!! mlib_m_ImageChannelExtract_*
 * functions must be used when they are implemented
 */
	for (i = 0; i < n; i++) {
		dst8[i] = src[nch * i];
	}
}

/* *********************************************************** */
static void
mlib_m_ImageGradient3x3U82D64_ext(
    __m64 * dst,
    mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_u8 *dst8 = (mlib_u8 *)dst;
	mlib_s32 i, val = src[0];

	for (i = 0; i < dx_l; i++)
		dst8[i] = val;
	mlib_m_ImageGradient3x3U82D64((__m64 *) (dst8 + dx_l), src,
	    n - dx_l - dx_r, nch);
	val = dst8[n - dx_r - 1];
	for (i = n - dx_r; i < n; i++)
		dst8[i] = val;
}

/* *********************************************************** */
static void
mlib_m_ImageGradient3x3Median_U8(
    mlib_u8 *dst,
    __m64 * src1,
    __m64 * src2,
    mlib_s32 *scale,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i, count = n >> 2, left = n & 3;
	mlib_s32 v_scale0 = scale[0] - 1;
	mlib_s32 v_scale1 = scale[1] - 1;
	__m64 val1, val2;

	val1 = (*src1++);
	val2 = (*src2++);

	for (i = 0; i < count; i++, dst += 4 * nch, src1++, src2++) {
		mlib_union_type res;
		__m64 tmp1, tmp2, res1, res2;
		mlib_s32 detl0, detl1, detl2, detl3;

		val1 = _mm_srai_pi16(val1, v_scale0);
		val2 = _mm_srai_pi16(val2, v_scale1);

		tmp1 = _mm_mullo_pi16(val1, val1);
		tmp2 = _mm_mulhi_pi16(val1, val1);
		res1 = _mm_unpacklo_pi16(tmp1, tmp2);
		res2 = _mm_unpackhi_pi16(tmp1, tmp2);
		tmp1 = _mm_mullo_pi16(val2, val2);
		tmp2 = _mm_mulhi_pi16(val2, val2);
		res1 = _mm_add_pi32(res1, _mm_unpacklo_pi16(tmp1, tmp2));
		res2 = _mm_add_pi32(res2, _mm_unpackhi_pi16(tmp1, tmp2));

		res1 =
		    _mm_packs_pi32(_mm_srai_pi32(res1, 1), _mm_srai_pi32(res2,
		    1));
		res.dv = _mm_add_pi16(res1, res1);

		val1 = src1[0];
		val2 = src2[0];
		detl0 = res.sv[0];
		detl1 = res.sv[1];
		detl2 = res.sv[2];
		detl3 = res.sv[3];

		FAST_SQRT_U8(dst, detl0);
		FAST_SQRT_U8(dst + nch, detl1);
		FAST_SQRT_U8(dst + 2 * nch, detl2);
		FAST_SQRT_U8(dst + 3 * nch, detl3);
	}

	if (left) {
		mlib_union_type res;
		__m64 tmp1, tmp2, res1, res2;

		val1 = _mm_srai_pi16(val1, v_scale0);
		val2 = _mm_srai_pi16(val2, v_scale1);

		tmp1 = _mm_mullo_pi16(val1, val1);
		tmp2 = _mm_mulhi_pi16(val1, val1);
		res1 = _mm_unpacklo_pi16(tmp1, tmp2);
		res2 = _mm_unpackhi_pi16(tmp1, tmp2);
		tmp1 = _mm_mullo_pi16(val2, val2);
		tmp2 = _mm_mulhi_pi16(val2, val2);
		res1 = _mm_add_pi32(res1, _mm_unpacklo_pi16(tmp1, tmp2));
		res2 = _mm_add_pi32(res2, _mm_unpackhi_pi16(tmp1, tmp2));

		res1 =
		    _mm_packs_pi32(_mm_srai_pi32(res1, 1), _mm_srai_pi32(res2,
		    1));
		res.dv = _mm_add_pi16(res1, res1);

		for (i = 0; i < left; i++, dst += nch) {
			mlib_s32 detl = res.sv[i];

			FAST_SQRT_U8(dst, detl);
		}
	}

	_mm_empty();
}

/* *********************************************************** */
static void
mlib_m_ImageGradient3x3Median_S16(
    mlib_s16 *dst,
    mlib_s32 *src0,
    mlib_s32 *src1,
    mlib_s32 *scale,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;
	mlib_s32 scale0 = scale[0];
	mlib_s32 scale1 = scale[1];

	for (i = 0; i < n; i++) {
		mlib_d64 s0, s1, rr;

		s0 = src0[i] >> scale0;
		s1 = src1[i] >> scale1;
		rr = mlib_sqrtf(s0 * s0 + s1 * s1);

		if (rr > MLIB_S16_MAX)
			rr = MLIB_S16_MAX;
		if (rr < MLIB_S16_MIN)
			rr = MLIB_S16_MIN;

		dst[i * nch] = rr;
	}
}

/* *********************************************************** */
void
mlib_ImageGradient3x3_U8_fast(
    mlib_image *dst,
    mlib_image *src,
    mlib_d64 *hmask,
    mlib_d64 *vmask,
    mlib_s32 *scale,
    mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_grad_ws ws_h[4], ws_v[4];
	__m64 *buff, *pbuff, *dsh, *dsv;
	mlib_u8 *dl = mlib_ImageGetData(dst);
	mlib_u8 *sl = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst);
	mlib_s32 slb = mlib_ImageGetStride(src);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nchan = mlib_ImageGetChannels(dst);
	void *sp;
	mlib_s32 j, k, wid4;

	wid4 = (dw + 7) / 4;
	pbuff = buff = mlib_malloc(sizeof (__m64) * (3 + 6 * nchan) * wid4);

	sp = pbuff;
	dsh = pbuff += wid4;
	dsv = pbuff += wid4;

	for (k = 0; k < nchan; k++) {
		ws_h[k].width = dw;
		ws_h[k].wid4 = wid4;
		ws_h[k].kern = (void *)hmask;
		ws_h[k].buff0 = pbuff += wid4;
		ws_h[k].buff1 = pbuff += wid4;
		ws_h[k].buff2 = pbuff += wid4;

		ws_v[k].width = dw;
		ws_v[k].wid4 = wid4;
		ws_v[k].kern = (void *)vmask;
		ws_v[k].buff0 = pbuff += wid4;
		ws_v[k].buff1 = pbuff += wid4;
		ws_v[k].buff2 = pbuff += wid4;
	}

	for (j = 0; j < 2; j++) {
		for (k = 0; k < nchan; k++)
			if (acmask[k]) {
				if (nchan == 1) {
					sp = sl;
				} else {
					mlib_m_ImageGradient3x3U82D64(sp,
					    sl + k, dw + 2, nchan);
				}

				mlib_m_grad3x3_prep(ws_h + k, j, sp);
				mlib_m_grad3x3_prep(ws_v + k, j, sp);
			}

		sl += slb;
	}

	for (j = 0; j < dh; j++) {
		for (k = 0; k < nchan; k++)
			if (acmask[k]) {
				if (nchan == 1) {
					sp = sl;
				} else {
					mlib_m_ImageGradient3x3U82D64(sp,
					    sl + k, dw + 2, nchan);
				}

				mlib_m_grad3x3_line(ws_h + k, dsh, sp);
				mlib_m_grad3x3_line(ws_v + k, dsv, sp);

				mlib_m_ImageGradient3x3Median_U8(dl + k, dsh,
				    dsv, scale, dw, nchan);
			}

		sl += slb;
		dl += dlb;
	}

	mlib_free(buff);

	_mm_empty();
}

/* *********************************************************** */
void
mlib_ImageGradient3x3_U8_ext_fast(
    mlib_image *dst,
    mlib_image *src,
    mlib_d64 *hmask,
    mlib_d64 *vmask,
    mlib_s32 *scale,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_grad_ws ws_h[4], ws_v[4];
	__m64 *buff, *pbuff, *dsh, *dsv;
	mlib_u8 *dl = mlib_ImageGetData(dst);
	mlib_u8 *sl = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst);
	mlib_s32 slb = mlib_ImageGetStride(src);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nchan = mlib_ImageGetChannels(dst);
	void *sp;
	mlib_s32 j, k, wid4;

	wid4 = (dw + 7) / 4;
	pbuff = buff = mlib_malloc(sizeof (__m64) * (3 + 6 * nchan) * wid4);

	sp = pbuff;
	dsh = pbuff += wid4;
	dsv = pbuff += wid4;

	for (k = 0; k < nchan; k++) {
		ws_h[k].width = dw;
		ws_h[k].wid4 = wid4;
		ws_h[k].kern = (void *)hmask;
		ws_h[k].buff0 = pbuff += wid4;
		ws_h[k].buff1 = pbuff += wid4;
		ws_h[k].buff2 = pbuff += wid4;

		ws_v[k].width = dw;
		ws_v[k].wid4 = wid4;
		ws_v[k].kern = (void *)vmask;
		ws_v[k].buff0 = pbuff += wid4;
		ws_v[k].buff1 = pbuff += wid4;
		ws_v[k].buff2 = pbuff += wid4;
	}

	for (j = 0; j < 2; j++) {
		for (k = 0; k < nchan; k++)
			if (acmask[k]) {
				mlib_m_ImageGradient3x3U82D64_ext(sp, sl + k,
				    dw + 2, nchan, dx_l, dx_r);

				mlib_m_grad3x3_prep(ws_h + k, j, sp);
				mlib_m_grad3x3_prep(ws_v + k, j, sp);
			}

		if ((j >= dy_t) && (j < dh + 1 - dy_b))
			sl += slb;
	}

	for (j = 0; j < dh; j++) {
		for (k = 0; k < nchan; k++)
			if (acmask[k]) {
				mlib_m_ImageGradient3x3U82D64_ext(sp, sl + k,
				    dw + 2, nchan, dx_l, dx_r);

				mlib_m_grad3x3_line(ws_h + k, dsh, sp);
				mlib_m_grad3x3_line(ws_v + k, dsv, sp);

				mlib_m_ImageGradient3x3Median_U8(dl + k, dsh,
				    dsv, scale, dw, nchan);
			}

		if ((j + 2 >= dy_t) && (j + 2 < dh + 1 - dy_b))
			sl += slb;
		dl += dlb;
	}

	mlib_free(buff);

	_mm_empty();
}

/* *********************************************************** */
mlib_status
mlib_ImageGradientKernVIS(
    mlib_d64 *dhmask,
    mlib_d64 *dvmask,
    mlib_s32 *iscale,
    mlib_d64 *hmask,
    mlib_d64 *vmask,
    mlib_d64 *offset,
    mlib_type type,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_s32 akernel[512], *ihmask = akernel, *ivmask;
	mlib_status reth, retv;
	mlib_s32 i, j;

	if ((m != 3) || (n != 3))
		return (MLIB_FAILURE);

	if (m * n > 256) {
		ihmask =
		    mlib_malloc(2 * m * n * (sizeof (mlib_s32) +
		    sizeof (mlib_d64)));

		if (ihmask == NULL)
			return (MLIB_FAILURE);
	}

	ivmask = ihmask + m * n;

	reth = __mlib_ImageConvKernelConvert(ihmask, iscale, hmask, m, n, type);
	retv =
	    __mlib_ImageConvKernelConvert(ivmask, iscale + 1, vmask, m, n,
	    type);

	if (!((reth == MLIB_SUCCESS) && (retv == MLIB_SUCCESS) &&
	    (iscale[0] < 32) && (iscale[0] >= 24) &&
	    (iscale[1] < 32) && (iscale[1] >= 24))) {

		if (ihmask != akernel)
			mlib_free(ihmask);
		return (MLIB_FAILURE);
	}

	iscale[0] -= 24;
	iscale[1] -= 24;

	for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++) {
			mlib_s32 ihval = ihmask[j * m + i] >> 16;
			mlib_s32 ivval = ivmask[j * m + i] >> 16;

			ihval = (ihval & 0xFFFF) | (ihval << 16);
			ivval = (ivval & 0xFFFF) | (ivval << 16);
			((mlib_s32 *)(dhmask + j * m + i))[0] = ihval;
			((mlib_s32 *)(dhmask + j * m + i))[1] = ihval;
			((mlib_s32 *)(dvmask + j * m + i))[0] = ivval;
			((mlib_s32 *)(dvmask + j * m + i))[1] = ivval;
		}
	}

	if (ihmask != akernel)
		mlib_free(ihmask);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
