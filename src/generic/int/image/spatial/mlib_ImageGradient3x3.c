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
 *  Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 *  Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_ImageGradient3x3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageGradient3x3   - Compute gradient magnitude of an image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageGradient3x3(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_d64   *hmask,
 *                                        const mlib_d64   *vmask,
 *                                        mlib_s32         cmask,
 *                                        mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      hmask     Pointer to horizontal mask in row-major order.
 *      vmask     Pointer to vertical mask in row-major order.
 *      top-left element in x and y directions respectively).
 *      cmask     Channel mask to indicate the channels to be processed.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *      edge      Type of edge condition.
 *
 * RESTRICTION
 *
 *  The src and the dst must be images of the same type.
 *  For the integral type function, the src and the dst can be images of
 *  MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT type.
 *
 *  The unselected channels are not overwritten. If both src and dst have
 *  just one channel, cmask is ignored.
 *
 *  The mapping of source image to destination image is left/top corner to
 *  left/top corner, but with a shift of the destination image if it's
 *  smaller than the source image.
 *  The exact mapping formula is
 *       xs = xd + x_offset
 *       ys = yd + y_offset
 *  where
 *       x_offset = (dst_width >= src_width) ? 0 : (kernel_size - 1)/2
 *       y_offset = (dst_height >= src_height) ? 0 : (kernel_size - 1)/2
 *
 *  The operation is applied on a per-channel basis.
 *  The src and dst must have same number of channels (1, 2, 3, or 4).
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_DST_COPY_SRC
 *    MLIB_EDGE_SRC_EXTEND
 *
 * DESCRIPTION
 *
 *  The "Gradient3x3" operation is an edge detector which
 *  computes the magnitude of the image gradient vector in two
 *  orthogonal directions.
 *
 *  The result of the "Gradient3x3" operation may be defined as:
 *
 *      dst[x][y][c] = ((SH(x, y,c))^2 + (SV(x, y,c))^2 )^0.5
 *
 *  where SH(x, y,c) and SV(x, y,c) are the horizontal and vertical
 *  gradient images generated from channel c of the source image by
 *  correlating it with the supplied orthogonal (horizontal and
 *  vertical) gradient masks.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCreate.h>
#include <mlib_SysMath.h>
#include <mlib_ImageDivTables.h>
#include <mlib_ImageSqrtTable.h>
#include <mlib_ImageGradientFunc.h>
#include <mlib_ImageConvEdge.h>
#include <mlib_ImageClipping.h>

/* *********************************************************** */
#ifndef _NO_LONGLONG
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageGradient3x3 = __mlib_ImageGradient3x3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageGradient3x3) mlib_ImageGradient3x3
    __attribute__((weak, alias("__mlib_ImageGradient3x3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_i_ImageGradient3x3MulAdd_U8(
	mlib_s64 *hdst,
	mlib_s64 *vdst,
	const mlib_u8 *src,
	const mlib_s32 *hfilter,
	const mlib_s32 *vfilter,
	mlib_s32 n,
	mlib_s32 nch);

static void mlib_ImageGradient3x3MulAdd_U8(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_u8 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch);


static void mlib_i_ImageGradient3x3Median_U8(
	mlib_u8 *dst,
	mlib_s64 *src1,
	mlib_s64 *src2,
	mlib_s32 n,
	mlib_s32 nch);

static void mlib_ImageGradient3x3Median_U8(
    mlib_u8 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa);

static void mlib_ImageGradient3x3U82F32_ext(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_ImageGradient3x3_U8_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa);

static void mlib_i_ImageGradient3x3MulAdd_S16(
	mlib_s64 *hdst,
	mlib_s64 *vdst,
	const mlib_s16 *src,
	const mlib_s32 *hfilter,
	const mlib_s32 *vfilter,
	mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3MulAdd_S16(
	mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_s16 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch);


static void mlib_i_ImageGradient3x3Median_S16(
    mlib_s16 *dst,
    mlib_s64 *src1,
    mlib_s64 *src2,
    mlib_s32 n,
    mlib_s32 nch);


static void mlib_ImageGradient3x3Median_S16(
    mlib_s16 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3_S16(
    mlib_image *dst,
    const mlib_image *src,
	const mlib_d64 *hmask,
	const mlib_d64 *vmask,
	const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa);

static void mlib_i_ImageGradient3x3MulAdd_S32(
    mlib_s64 *hdst,
    mlib_s64 *vdst,
    const mlib_s32 *src,
	const mlib_s32 *hfilter,
	const mlib_s32 *vfilter,
	mlib_s32 n);

static void mlib_ImageGradient3x3MulAdd_S32(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_s32 *src,
	const mlib_d64 *hfilter,
	const mlib_d64 *vfilter,
	mlib_s32 n);

static void mlib_ImageGradient3x3S162S32_ext(
    mlib_s32 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_ImageGradient3x3_S16_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa);

static void mlib_i_ImageGradient3x3MulAdd_D64(
    mlib_s64 *hdst,
    mlib_s64 *vdst,
    const mlib_s32 *src,
    const mlib_s64 *hfilter,
    const mlib_s64 *vfilter,
	const mlib_s64 *lherro,
	const mlib_s64 *lverro,
	mlib_s32 n,
    mlib_s32 nch);


static void mlib_ImageGradient3x3MulAdd_D64(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_s32 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_i_ImageGradient3x3Median_S32(
    mlib_s32 *dst,
    mlib_s64 *src1,
    mlib_s64 *src2,
    mlib_s32 n,
    mlib_s32 nch);


static void mlib_ImageGradient3x3Median_S32(
    mlib_s32 *dst,
    mlib_d64 *src1,
    mlib_d64 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa);

static void mlib_ImageGradient3x3S322S32_ext(
    mlib_s32 *dst,
    const mlib_s32 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_ImageGradient3x3_S32_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa);

static void mlib_i_ImageGradient3x3MulAdd_U16(
    mlib_s64 *hdst,
    mlib_s64 *vdst,
    const mlib_u16 *src,
    const mlib_s32 *hfilter,
    const mlib_s32 *vfilter,
    mlib_s32 n,
    mlib_s32 nch);


static void mlib_ImageGradient3x3MulAdd_U16(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_u16 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_i_ImageGradient3x3Median_U16(
    mlib_u16 *dst,
    mlib_s64 *src1,
    mlib_s64 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3Median_U16(
    mlib_u16 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa);

static void mlib_ImageGradient3x3U162S32_ext(
    mlib_s32 *dst,
    const mlib_u16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_ImageGradient3x3_U16_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa);

/* *********************************************************** */
/* define functions for sqrt operation */
static mlib_u32
mlib_lsqrt(mlib_u64 x) /* x >= 0 */
{
	mlib_u64 x1;
	mlib_s64 s, g0, g1;
	if (x <= 1)
		return (x);
	s = 1;
	x1 = x - 1;
	if (x1 > 4294967295U) {s = s + 16; x1 = x1 >> 32; }
	if (x1 > 65535) {s = s + 8; x1 = x1 >> 16; }
	if (x1 > 255) {s = s + 4; x1 = x1 >> 8; }
	if (x1 > 15) {s = s + 2; x1 = x1 >> 4; }
	s = (x1 > 3) ? (s + 1) : s;
	g0 = (mlib_s64)1 << s;
	g1 = (g0 + (x >> s)) >> 1;
	while (g1 < g0) {
		g0 = g1;
		g1 = (g0 + (x / g0)) >> 1;
	}
	return (g0);
}

static mlib_s64
mlib_d2isqrt(mlib_d64 x)
{
	/* be care of the data range of input x */
	/* although  its type is double */
	/* it's better that limit x <= MLIB_S64_MAX * MLIB_S64_MAX */
	/* of course , should make sure of x >= 0 */
	mlib_s64 tempx = *((mlib_s64 *) &x);
	mlib_s64 ret;
	mlib_u64 base = (tempx & 0xFFFFFFFFFFFFF | 0x10000000000000) << 10;
	mlib_s64 ext = ((tempx >> 52) & 0x7FF) - 1023;
	if (ext < 0)
		return (0);
	if (ext & 1) {
		ext -= 1;
		base <<= 1;
	}
	ext = 31 - (ext >> 1);
	ret = mlib_lsqrt(base);
	ret = (((ext >> 63) & (ret << (-ext))) |
			(~(ext >> 63) & (ret >> ext)));
	return (ret);
}
/* *********************************************************** */


mlib_status
__mlib_ImageGradient3x3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_type type;

	MLIB_IMAGE_CHECK(dst);
	type = mlib_ImageGetType(dst);

	if (type != MLIB_BYTE && type != MLIB_SHORT && type != MLIB_USHORT &&
	    type != MLIB_INT) {
		return (MLIB_FAILURE);
	}

	return mlib_ImageGradient3x3_alltypes(dst, src, hmask, vmask, cmask,
	    edge);
}

/* *********************************************************** */

#define	MLIB_D2_16	65536.0f

#define	MLIB_U8_SCALE	10
#define	MLIB_U8_SCALE_HALF	512
#define	MLIB_U8_SCALE_POW	1048576.0f
#define	MLIB_U8_SCALE_INV	9.765625e-4 /*  1.0 / (1 << MLIB_U8_SCALE) */

#define	MLIB_S16_SCALE	20
#define	MLIB_S16_SCALE_HALF	524288
#define	MLIB_S16_SCALE_INV	9.536743164e-7

#define	MLIB_S32_SCALE	26
#define	MLIB_S32_ERROR	12
#define	MLIB_S32_SCALE_HALF	33554432
#define	MLIB_S32_SCALE_INV	1.49011611938e-8

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING /* equal to ifdef platform==sparc */

/* *********************************************************** */

#define	SAT_U8(DST, SRC)  DST = ((mlib_s32) ((SRC) * 32768.0f)) >> 15
/* *********************************************************** */

#define	SAT_S16(DST, SRC)	DST = ((mlib_s32) (SRC)) >> 16

#define	CLAMP_S16(DST, SRC)                                       \
	if ((SRC) > MLIB_S16_MAX) {                                 \
	    (SRC) = MLIB_S16_MAX;                                   \
	}                                                       \
	DST = ((mlib_s16)(SRC))

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	DST = (((mlib_s32)((SRC) - sat)) >> 16) ^ 0x8000

#define	CLAMP_U16(DST, SRC)                                       \
	if ((SRC) > MLIB_U16_MAX) {                                 \
	    (SRC) = MLIB_U16_MAX;                                   \
	}                                                       \
	DST = ((mlib_u16)(SRC))

/* *********************************************************** */

#define	SAT_S32(DST, SRC)	DST = (mlib_s32) (SRC)
#define	CLAMP_S32(DST, SRC)                                       \
	if ((SRC) > MLIB_S32_MAX) {                                 \
	    (SRC) = MLIB_S32_MAX;                                   \
	}                                                       \
	DST = ((mlib_s32)(SRC))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SAT_U8(DST, SRC)                                        \
	if ((SRC) >= MLIB_U16_MAX) {                            \
	    (SRC) = MLIB_U16_MAX;                               \
	}                                                       \
	DST = (mlib_s32)(SRC)

/* *********************************************************** */

#define	SAT_S16(DST, SRC)                                       \
	if ((SRC) > fmax_s32) {                                 \
	    (SRC) = fmax_s32;                                   \
	}                                                       \
	DST = ((mlib_s32)(SRC)) >> 16

#define	CLAMP_S16(DST, SRC)                                       \
	if ((SRC) > MLIB_S16_MAX) {                                 \
	    (SRC) = MLIB_S16_MAX;                                   \
	}                                                       \
	DST = ((mlib_s16)(SRC))

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	(SRC) -= sat;                                           \
	if ((SRC) >= fmax_s32) {                                \
	    (SRC) = fmax_s32;                                   \
	}                                                       \
	DST = (((mlib_s32)(SRC)) >> 16) ^ 0x8000

#define	CLAMP_U16(DST, SRC)                                       \
	if ((SRC) > MLIB_U16_MAX) {                                 \
	    (SRC) = MLIB_U16_MAX;                                   \
	}                                                       \
	DST = ((mlib_u16)(SRC))

/* *********************************************************** */

#define	SAT_S32(DST, SRC)                                       \
	if ((SRC) > fmax_s32) {                                 \
	    (SRC) = fmax_s32;                                   \
	}                                                       \
	DST = ((mlib_s32)(SRC))

#define	CLAMP_S32(DST, SRC)                                       \
	if ((SRC) > MLIB_S32_MAX) {                                 \
	    (SRC) = MLIB_S32_MAX;                                   \
	}                                                       \
	DST = ((mlib_s32)(SRC))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

void
mlib_i_ImageGradient3x3MulAdd_U8(
	mlib_s64 *hdst,
	mlib_s64 *vdst,
	const mlib_u8 *src,
    const mlib_s32 *hfilter,
	const mlib_s32 *vfilter,
	mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 iscale = MLIB_U8_SCALE;
	mlib_f32 fscale = MLIB_U8_SCALE_INV;
	mlib_s64 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_u8 *src2 = src + 2 * nch;
	mlib_s32 hval0 = hfilter[0];
	mlib_s32 vval0 = vfilter[0];
	mlib_s32 hval1 = hfilter[1];
	mlib_s32 vval1 = vfilter[1];
	mlib_s32 hval2 = hfilter[2];
	mlib_s32 vval2 = vfilter[2];
	mlib_s32 val0 = src[0];
	mlib_s32 val1 = src[nch];
	mlib_s64 hdvl = hdst[0];
	mlib_s64 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_s32 hdvl0 = val0 * hval0 + hdvl;
		mlib_s32 vdvl0 = val0 * vval0 + vdvl;
		mlib_s32 val2 = src2[i * nch];

		hdvl = hdst1[i];
		vdvl = vdst1[i];

		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}
/* *********************************************************** */

void
mlib_ImageGradient3x3MulAdd_U8(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_u8 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_f32 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_u8 *src2 = src + 2 * nch;
	mlib_f32 hval0 = (mlib_f32)hfilter[0];
	mlib_f32 vval0 = (mlib_f32)vfilter[0];
	mlib_f32 hval1 = (mlib_f32)hfilter[1];
	mlib_f32 vval1 = (mlib_f32)vfilter[1];
	mlib_f32 hval2 = (mlib_f32)hfilter[2];
	mlib_f32 vval2 = (mlib_f32)vfilter[2];
	mlib_f32 val0 = mlib_U82F32[src[0]];
	mlib_f32 val1 = mlib_U82F32[src[nch]];
	mlib_f32 hdvl = hdst[0];
	mlib_f32 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 hdvl0 = val0 * hval0 + hdvl;
		mlib_f32 vdvl0 = val0 * vval0 + vdvl;
		mlib_f32 val2 = mlib_U82F32[src2[i * nch]];

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}


/* *********************************************************** */

#define	FAST_SQRT_U8(dp, v)                                     \
	{                                                       \
	    mlib_s32 diap;                                      \
	                                                        \
	    diap = mlib_SqrtTable_U8_Diap[(v) >> 8];            \
	    *(dp) = mlib_SqrtTable_U8[((v) >> diap) + diap];    \
	}

/* *********************************************************** */

void
mlib_i_ImageGradient3x3Median_U8(
    mlib_u8 *dst,
	mlib_s64 *src1,
	mlib_s64 *src2,
	mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 norm = (src1[i] * src1[i] + src2[i] * src2[i]) /
							MLIB_U8_SCALE_POW;
		mlib_s32 detl;

		SAT_U8(detl, norm);
		src1[i] = MLIB_U8_SCALE_HALF;
		src2[i] = MLIB_U8_SCALE_HALF;
		FAST_SQRT_U8(dst + i * nch, detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3Median_U8(
    mlib_u8 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 norm = (src1[i] * src1[i] + src2[i] * src2[i]);
		mlib_s32 detl;

		SAT_U8(detl, norm);

		src1[i] = 0.5f;
		src2[i] = 0.5f;
		FAST_SQRT_U8(dst + i * nch, detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa)
{
	mlib_s64 *lsh, *lsv;
	mlib_s64 *llsa = (mlib_s64 *)lsa;
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_u8 *da = mlib_ImageGetData(dst);
	mlib_u8 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst);
	mlib_s32 slb = mlib_ImageGetStride(src);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;
	mlib_s32 ihmask[9], ivmask[9];
	mlib_s32 iscale = MLIB_U8_SCALE;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;
	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	lsh = llsa + dw + 3;
	lsv = lsh + dw;
	for (i = 0; i < dw; i++) {
		lsh[i] = MLIB_U8_SCALE_HALF;
		lsv[i] = MLIB_U8_SCALE_HALF;
	}

	for (i = 0; i < 9; i++) {
		ihmask[i] = (mlib_s32)(hmask[i] * (1 << iscale));
		ivmask[i] = (mlib_s32)(vmask[i] * (1 << iscale));
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				const mlib_u8 *sa1 = sa + k;
				const mlib_s32 *hmask1 = ihmask;
				const mlib_s32 *vmask1 = ivmask;
				for (j1 = 0; j1 < 3; j1++, sa1 += slb,
					hmask1 += 3, vmask1 += 3) {
					mlib_i_ImageGradient3x3MulAdd_U8(lsh,
						lsv, sa1, hmask1,
						vmask1, dw, nch);
				}

				mlib_i_ImageGradient3x3Median_U8(da + k,
						lsh, lsv, dw, nch);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3U82F32_ext(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_s32 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U8_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa)
{
	mlib_s64 *lsh, *lsv;
	mlib_s64 *llsa = (mlib_s64 *)lsa;
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_u8 *usa = (mlib_u8 *)dsa;
	mlib_u8 *da = mlib_ImageGetData(dst);
	mlib_u8 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst);
	mlib_s32 slb = mlib_ImageGetStride(src);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;
	mlib_s32 ihmask[9], ivmask[9];
	mlib_s32 iscale = MLIB_U8_SCALE;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;

	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	lsh = llsa + dw +3;
	lsv = lsh +dw;
	for (i = 0; i < dw; i++) {
		lsh[i] = MLIB_U8_SCALE_HALF;
		lsv[i] = MLIB_U8_SCALE_HALF;
	}


	for (i = 0; i < 9; i++) {
		ihmask[i] = (mlib_s32)(hmask[i] * (1 << iscale));
		ivmask[i] = (mlib_s32)(vmask[i] * (1 << iscale));
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_u8 *sa1 = sa + k;
				const mlib_s32 *hmask1 = ihmask;
				const mlib_s32 *vmask1 = ivmask;

				for (j1 = 0; j1 < 3;
				    j1++, hmask1 += 3, vmask1 += 3) {
					mlib_ImageGradient3x3U82F32_ext(usa,
					    sa1, dw + 2, nch, dx_l, dx_r);

					mlib_i_ImageGradient3x3MulAdd_U8(lsh,
						lsv, usa, hmask1,
						vmask1, dw, 1);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + 1 - dy_b))
						sa1 += slb;
				}

				mlib_i_ImageGradient3x3Median_U8(da + k,
					lsh, lsv, dw, nch);
			}
		}

		if ((j >= dy_t) && (j < dh + 1 - dy_b))
			sa += slb;
	}
}

/* *********************************************************** */

void
mlib_i_ImageGradient3x3MulAdd_S16(
	mlib_s64 *hdst,
	mlib_s64 *vdst,
	const mlib_s16 *src,
	const mlib_s32 *hfilter,
	const mlib_s32 *vfilter,
	mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s64 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_s16 *src2 = src + 2 * nch;
	mlib_s64 hval0 = hfilter[0];
	mlib_s64 vval0 = vfilter[0];
	mlib_s64 hval1 = hfilter[1];
	mlib_s64 vval1 = vfilter[1];
	mlib_s64 hval2 = hfilter[2];
	mlib_s64 vval2 = vfilter[2];
	mlib_s64 val0 = src[0];
	mlib_s64 val1 = src[nch];
	mlib_s64 hdvl = hdst[0];
	mlib_s64 vdvl = vdst[0];
	mlib_s32 i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_s64 val2 = src2[nch * i];
		mlib_s64 hdvl0 = val0 * hval0 + hdvl;
		mlib_s64 vdvl0 = val0 * vval0 + vdvl;

		hdvl = hdst1[i];
		vdvl = vdst1[i];

		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}


/* *********************************************************** */

void
mlib_ImageGradient3x3MulAdd_S16(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_s16 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_f32 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_s16 *src2 = src + 2 * nch;
	mlib_f32 hval0 = (mlib_f32)hfilter[0];
	mlib_f32 vval0 = (mlib_f32)vfilter[0];
	mlib_f32 hval1 = (mlib_f32)hfilter[1];
	mlib_f32 vval1 = (mlib_f32)vfilter[1];
	mlib_f32 hval2 = (mlib_f32)hfilter[2];
	mlib_f32 vval2 = (mlib_f32)vfilter[2];
	mlib_f32 val0 = src[0];
	mlib_f32 val1 = src[nch];
	mlib_f32 hdvl = hdst[0];
	mlib_f32 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 val2 = src2[nch * i];
		mlib_f32 hdvl0 = val0 * hval0 + hdvl;
		mlib_f32 vdvl0 = val0 * vval0 + vdvl;

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}
/* *********************************************************** */

void
mlib_i_ImageGradient3x3Median_S16(
    mlib_s16 *dst,
    mlib_s64 *src1,
    mlib_s64 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 tmp1 = src1[i] * MLIB_S16_SCALE_INV;
		mlib_f32 tmp2 = src2[i] * MLIB_S16_SCALE_INV;
		mlib_s64 detl = mlib_d2isqrt(tmp1 * tmp1 + tmp2 * tmp2);
		src1[i] = MLIB_S16_SCALE_HALF;
		src2[i] = MLIB_S16_SCALE_HALF;
		CLAMP_S16(dst[i*nch], detl);
	}
}


/* *********************************************************** */

void
mlib_ImageGradient3x3Median_S16(
    mlib_s16 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_f32 fmax_s32 = (mlib_f32)(MLIB_S32_MAX - 64);
#endif /* MLIB_USE_FTOI_CLAMPING */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 detl =
		    (mlib_sqrtf(src1[i] * src1[i] +
		    src2[i] * src2[i]) * MLIB_D2_16);
		src1[i] = 0.5f;
		src2[i] = 0.5f;
		SAT_S16(dst[i * nch], detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_S16(
    mlib_image *dst,
    const mlib_image *src,
	const mlib_d64 *hmask,
	const mlib_d64 *vmask,
	const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa)
{
	mlib_s64 *lsh, *lsv;
	mlib_s64 *llsa = (mlib_s64 *)lsa;
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_s16 *da = mlib_ImageGetData(dst);
	mlib_s16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;
	mlib_s32 ihmask[9], ivmask[9];
	mlib_s32 iscale = MLIB_S16_SCALE;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;
	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	lsh = llsa + dw + 3;
	lsv = lsh + dw;
	for (i = 0; i < dw; i++) {
		lsh[i] = MLIB_S16_SCALE_HALF;
		lsv[i] = MLIB_S16_SCALE_HALF;
	}

	for (i = 0; i < 9; i++) {
		ihmask[i] = (mlib_s32)(hmask[i] * (1 << iscale));
		ivmask[i] = (mlib_s32)(vmask[i] * (1 << iscale));
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_s16 *sa1 = sa + k;
				const mlib_s32 *hmask1 = ihmask;
				const mlib_s32 *vmask1 = ivmask;
				for (j1 = 0; j1 < 3;
				    j1++, sa1 += slb, hmask1 += 3, vmask1 += 3)
					mlib_i_ImageGradient3x3MulAdd_S16(lsh,
					    lsv, sa1, hmask1,
						vmask1, dw, nch);

					mlib_i_ImageGradient3x3Median_S16(
						da + k, lsh, lsv, dw, nch);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_i_ImageGradient3x3MulAdd_S32(
    mlib_s64 *hdst,
    mlib_s64 *vdst,
	const mlib_s32 *src,
	const mlib_s32 *hfilter,
	const mlib_s32 *vfilter,
	mlib_s32 n)
{
	mlib_s64 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_s32 *src2 = src + 2;
	mlib_s64 hval0 = hfilter[0];
	mlib_s64 vval0 = vfilter[0];
	mlib_s64 hval1 = hfilter[1];
	mlib_s64 vval1 = vfilter[1];
	mlib_s64 hval2 = hfilter[2];
	mlib_s64 vval2 = vfilter[2];
	mlib_s64 val0 = src[0];
	mlib_s64 val1 = src[1];
	mlib_s64 hdvl = hdst[0];
	mlib_s64 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_s64 val2 = src2[i];
		mlib_s64 hdvl0 = val0 * hval0 + hdvl;
		mlib_s64 vdvl0 = val0 * vval0 + vdvl;


		hdvl = hdst1[i];
		vdvl = vdst1[i];

		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3MulAdd_S32(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_s32 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n)
{
	mlib_f32 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_s32 *src2 = src + 2;
	mlib_f32 hval0 = (mlib_f32)hfilter[0];
	mlib_f32 vval0 = (mlib_f32)vfilter[0];
	mlib_f32 hval1 = (mlib_f32)hfilter[1];
	mlib_f32 vval1 = (mlib_f32)vfilter[1];
	mlib_f32 hval2 = (mlib_f32)hfilter[2];
	mlib_f32 vval2 = (mlib_f32)vfilter[2];
	mlib_f32 val0 = (mlib_f32)src[0];
	mlib_f32 val1 = (mlib_f32)src[1];
	mlib_f32 hdvl = hdst[0];
	mlib_f32 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 val2 = (mlib_f32)src2[i];
		mlib_f32 hdvl0 = val0 * hval0 + hdvl;
		mlib_f32 vdvl0 = val0 * vval0 + vdvl;

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3S162S32_ext(
    mlib_s32 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_s32 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (i = n - dx_r; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_S16_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa)
{
	mlib_s64 *lsh, *lsv;
	mlib_s64 *llsa = (mlib_s64 *)lsa;
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_s32 *isa = (mlib_s32 *)fsa;
	mlib_s16 *da = mlib_ImageGetData(dst);
	mlib_s16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;
	mlib_s32 ihmask[9], ivmask[9];
	mlib_s32 iscale = MLIB_S16_SCALE;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;
	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	lsh = llsa + dw + 3;
	lsv = lsh + dw;
	for (i = 0; i < dw; i++) {
		lsh[i] = MLIB_S16_SCALE_HALF;
		lsv[i] = MLIB_S16_SCALE_HALF;
	}

	for (i = 0; i < 9; i++) {
		ihmask[i] = (mlib_s32)(hmask[i] * (1 << iscale));
		ivmask[i] = (mlib_s32)(vmask[i] * (1 << iscale));
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_s16 *sa1 = sa + k;
				const mlib_s32 *hmask1 = ihmask;
				const mlib_s32 *vmask1 = ivmask;

				for (j1 = 0; j1 < 3;
				    j1++, hmask1 += 3, vmask1 += 3) {
					mlib_ImageGradient3x3S162S32_ext(isa,
					    sa1, dw + 2, nch, dx_l, dx_r);
					mlib_i_ImageGradient3x3MulAdd_S32(lsh,
					    lsv, isa, hmask1, vmask1, dw);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + 1 - dy_b))
						sa1 += slb;
				}

				mlib_i_ImageGradient3x3Median_S16(da + k, lsh,
				    lsv, dw, nch);
			}
		}

		if ((j >= dy_t) && (j < dh + 1 - dy_b))
			sa += slb;
	}
}

/* *********************************************************** */

void
mlib_i_ImageGradient3x3MulAdd_D64(
    mlib_s64 *hdst,
    mlib_s64 *vdst,
	const mlib_s32 *src,
    const mlib_s64 *hfilter,
    const mlib_s64 *vfilter,
    const mlib_s64 *lherro,
	const mlib_s64 *lverro,
	mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s64 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_s32 *src2 = src + 2 * nch;
	mlib_s64 hval0 = hfilter[0];
	mlib_s64 vval0 = vfilter[0];
	mlib_s64 hval1 = hfilter[1];
	mlib_s64 vval1 = vfilter[1];
	mlib_s64 hval2 = hfilter[2];
	mlib_s64 vval2 = vfilter[2];
	mlib_s64 herr0 = lherro[0];
	mlib_s64 verr0 = lverro[0];
	mlib_s64 herr1 = lherro[1];
	mlib_s64 verr1 = lverro[1];
	mlib_s64 herr2 = lherro[2];
	mlib_s64 verr2 = lverro[2];
	mlib_s64 val0 = src[0];
	mlib_s64 val1 = src[nch];
	mlib_s64 hdvl = hdst[0];
	mlib_s64 vdvl = vdst[0];
	mlib_s32 i;
	mlib_s32 ierr_scale = MLIB_S32_ERROR;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_s64 hdvl0 = val0 * hval0 + hdvl +
			((val0 * herr0) >> ierr_scale);
		mlib_s64 vdvl0 = val0 * vval0 + vdvl +
			((val0 * verr0) >> ierr_scale);
		mlib_s64 val2 = src2[i * nch];

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1 + ((val1 * herr1) >> ierr_scale);
		vdvl0 += val1 * vval1 + ((val1 * verr1) >> ierr_scale);
		hdvl0 += val2 * hval2 + ((val2 * herr2) >> ierr_scale);
		vdvl0 += val2 * vval2 + ((val2 * verr2) >> ierr_scale);
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */


void
mlib_ImageGradient3x3MulAdd_D64(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_s32 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_d64 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_s32 *src2 = src + 2 * nch;
	mlib_d64 hval0 = hfilter[0];
	mlib_d64 vval0 = vfilter[0];
	mlib_d64 hval1 = hfilter[1];
	mlib_d64 vval1 = vfilter[1];
	mlib_d64 hval2 = hfilter[2];
	mlib_d64 vval2 = vfilter[2];
	mlib_d64 val0 = src[0];
	mlib_d64 val1 = src[nch];
	mlib_d64 hdvl = hdst[0];
	mlib_d64 vdvl = vdst[0];
	mlib_s32 i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_d64 hdvl0 = val0 * hval0 + hdvl;
		mlib_d64 vdvl0 = val0 * vval0 + vdvl;
		mlib_d64 val2 = src2[i * nch];

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */

void
mlib_i_ImageGradient3x3Median_S32(
    mlib_s32 *dst,
    mlib_s64 *src1,
    mlib_s64 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_d64 tmp1 = src1[i] * (mlib_d64)MLIB_S32_SCALE_INV;
		mlib_d64 tmp2 = src2[i] * (mlib_d64)MLIB_S32_SCALE_INV;
		mlib_s64 detl = mlib_d2isqrt(tmp1 * tmp1 + tmp2 * tmp2);
		src1[i] = MLIB_S32_SCALE_HALF;
		src2[i] = MLIB_S32_SCALE_HALF;
		CLAMP_S32(dst[i * nch], detl);
	}
}

/* *********************************************************** */
void
mlib_ImageGradient3x3Median_S32(
    mlib_s32 *dst,
    mlib_d64 *src1,
    mlib_d64 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_d64 fmax_s32 = (mlib_d64)(MLIB_S32_MAX);
#endif /* MLIB_USE_FTOI_CLAMPING */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_d64 detl =
		    mlib_sqrt(src1[i] * src1[i] + src2[i] * src2[i]);
		src1[i] = 0.5;
		src2[i] = 0.5;
		SAT_S32(dst[i * nch], detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa)
{
	mlib_s64 *lsh, *lsv;
	mlib_s64 *llsa = (mlib_s64 *)lsa;
	mlib_d64 *dsh, *dsv;
	mlib_s32 *da = mlib_ImageGetData(dst);
	mlib_s32 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 2;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 2;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;
	mlib_s64 ihmask[9], ivmask[9];
	mlib_s32 iscale = MLIB_S32_SCALE;
	mlib_s32 ierr_scale = MLIB_S32_ERROR;
	mlib_s64 lherro[9], lverro[9];

	dsh = (void *)(dsa + dw + 3);
	dsv = dsh + dw;
	for (i = 0; i < dw; i++) {
		dsh[i] = 0.5;
		dsv[i] = 0.5;
	}

	lsh = (void *)(llsa + dw + 3);
	lsv = lsh + dw;
	for (i = 0; i < dw; i++) {
		lsh[i] = MLIB_S32_SCALE_HALF;
		lsv[i] = MLIB_S32_SCALE_HALF;
	}

	for (i = 0; i < 9; i++) {
		ihmask[i] = (mlib_s64)(hmask[i] * ((mlib_s64)1 << iscale));
		ivmask[i] = (mlib_s64)(vmask[i] * ((mlib_s64)1 << iscale));
		lherro[i] = (mlib_s64)(hmask[i] *
				((mlib_s64) 1<< (iscale + ierr_scale))) -
				(ihmask[i] * (1 << ierr_scale));
		lverro[i] = (mlib_s64)(vmask[i] *
				((mlib_s64)1 << (iscale + ierr_scale))) -
				(ivmask[i] * (1 << ierr_scale));
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_s32 *sa1 = sa + k;
				const mlib_s64 *hmask1 = ihmask;
				const mlib_s64 *vmask1 = ivmask;
				const mlib_s64 *lherro1 = lherro;
				const mlib_s64 *lverro1 = lverro;
				for (j1 = 0; j1 < 3;
				    j1++, sa1 += slb, hmask1 += 3,
					vmask1 += 3, lherro1 += 3, lverro1 += 3)
					mlib_i_ImageGradient3x3MulAdd_D64(lsh,
					    lsv, sa1, hmask1,
						vmask1, lherro1,
						lverro1, dw, nch);

					mlib_i_ImageGradient3x3Median_S32(
						da + k, lsh, lsv, dw, nch);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3S322S32_ext(
    mlib_s32 *dst,
    const mlib_s32 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_s32 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_S32_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa)
{
	mlib_s64 *lsh, *lsv;
	mlib_s64 *llsa = (mlib_s64 *)lsa;
	mlib_d64 *dsh, *dsv;
	mlib_s32 *isa = (mlib_s32 *)dsa;
	mlib_s32 *da = mlib_ImageGetData(dst);
	mlib_s32 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 2;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 2;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;
	mlib_s64 ihmask[9], ivmask[9];
	mlib_s32 iscale = MLIB_S32_SCALE;
	mlib_s32 ierr_scale = MLIB_S32_ERROR;
	mlib_s64 lherro[9], lverro[9];

	dsh = (void *)(dsa + dw + 3);
	dsv = dsh + dw;
	for (i = 0; i < dw; i++) {
		dsh[i] = 0.5;
		dsv[i] = 0.5;
	}

	lsh = (void *)(llsa + dw + 3);
	lsv = lsh + dw;
	for (i = 0; i < dw; i++) {
		lsh[i] = MLIB_S32_SCALE_HALF;
		lsv[i] = MLIB_S32_SCALE_HALF;
	}

	for (i = 0; i < 9; i++) {
		ihmask[i] = (mlib_s64)(hmask[i] * ((mlib_s64)1 << iscale));
		ivmask[i] = (mlib_s64)(vmask[i] * ((mlib_s64)1 << iscale));
		lherro[i] = (mlib_s64)(hmask[i] *
				((mlib_s64) 1<< (iscale + ierr_scale))) -
				(ihmask[i] * (1 << ierr_scale));
		lverro[i] = (mlib_s64)(vmask[i] *
				((mlib_s64)1 << (iscale + ierr_scale))) -
				(ivmask[i] * (1 << ierr_scale));
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_s32 *sa1 = sa + k;
				const mlib_s64 *hmask1 = ihmask;
				const mlib_s64 *vmask1 = ivmask;
				const mlib_s64 *lherro1 = lherro;
				const mlib_s64 *lverro1 = lverro;

				for (j1 = 0; j1 < 3;
				    j1++, hmask1 += 3, vmask1 += 3,
					lherro1 += 3, lverro1 += 3) {
					mlib_ImageGradient3x3S322S32_ext(isa,
					    sa1, dw + 2, nch, dx_l, dx_r);

					mlib_i_ImageGradient3x3MulAdd_D64(lsh,
					    lsv, isa, hmask1,
						vmask1, lherro1,
						lverro1, dw, 1);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + 1 - dy_b))
						sa1 += slb;
				}

				mlib_i_ImageGradient3x3Median_S32(da + k,
						lsh, lsv, dw, nch);

			}
		}

		if ((j >= dy_t) && (j < dh + 1 - dy_b))
			sa += slb;
	}
}

/* *********************************************************** */

void
mlib_i_ImageGradient3x3MulAdd_U16(
    mlib_s64 *hdst,
    mlib_s64 *vdst,
    const mlib_u16 *src,
    const mlib_s32 *hfilter,
    const mlib_s32 *vfilter,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s64 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_u16 *src2 = src + 2 * nch;
	mlib_s64 hval0 = hfilter[0];
	mlib_s64 vval0 = vfilter[0];
	mlib_s64 hval1 = hfilter[1];
	mlib_s64 vval1 = vfilter[1];
	mlib_s64 hval2 = hfilter[2];
	mlib_s64 vval2 = vfilter[2];
	mlib_s64 val0 = src[0];
	mlib_s64 val1 = src[nch];
	mlib_s64 hdvl = hdst[0];
	mlib_s64 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_s64 val2 = src2[nch * i];
		mlib_s64 hdvl0 = val0 * hval0 + hdvl;
		mlib_s64 vdvl0 = val0 * vval0 + vdvl;

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */


void
mlib_ImageGradient3x3MulAdd_U16(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_u16 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_f32 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_u16 *src2 = src + 2 * nch;
	mlib_f32 hval0 = (mlib_f32)hfilter[0];
	mlib_f32 vval0 = (mlib_f32)vfilter[0];
	mlib_f32 hval1 = (mlib_f32)hfilter[1];
	mlib_f32 vval1 = (mlib_f32)vfilter[1];
	mlib_f32 hval2 = (mlib_f32)hfilter[2];
	mlib_f32 vval2 = (mlib_f32)vfilter[2];
	mlib_f32 val0 = src[0];
	mlib_f32 val1 = src[nch];
	mlib_f32 hdvl = hdst[0];
	mlib_f32 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 val2 = src2[nch * i];
		mlib_f32 hdvl0 = val0 * hval0 + hdvl;
		mlib_f32 vdvl0 = val0 * vval0 + vdvl;

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */

void
mlib_i_ImageGradient3x3Median_U16(
    mlib_u16 *dst,
    mlib_s64 *src1,
    mlib_s64 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 tmp1 = src1[i] * MLIB_S16_SCALE_INV;
		mlib_f32 tmp2 = src2[i] * MLIB_S16_SCALE_INV;
		mlib_s64 detl = mlib_d2isqrt(tmp1 * tmp1 + tmp2 * tmp2);
		src1[i] = MLIB_S16_SCALE_HALF;
		src2[i] = MLIB_S16_SCALE_HALF;
		CLAMP_U16(dst[i * nch], detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3Median_U16(
    mlib_u16 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;
	mlib_f32 sat = (mlib_f32)(0x80000000);

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_f32 fmax_s32 = (mlib_f32)(MLIB_S32_MAX - 64);
#endif /* MLIB_USE_FTOI_CLAMPING */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 detl =
		    (mlib_sqrtf(src1[i] * src1[i] +
		    src2[i] * src2[i]) * MLIB_D2_16);
		src1[i] = 0.5f;
		src2[i] = 0.5f;
		SAT_U16(dst[i * nch], detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa)
{
	mlib_s64 *lsh, *lsv;
	mlib_s64 *llsa = (mlib_s64 *)lsa;
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_u16 *da = mlib_ImageGetData(dst);
	mlib_u16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;
	mlib_s32 ihmask[9], ivmask[9];
	mlib_s32 iscale = MLIB_S16_SCALE;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;
	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	lsh = llsa + dw + 3;
	lsv = lsh + dw;
	for (i = 0; i < dw; i++) {
		lsh[i] = MLIB_S16_SCALE_HALF;
		lsv[i] = MLIB_S16_SCALE_HALF;
	}

	for (i = 0; i < 9; i++) {
		ihmask[i] = (mlib_s32)(hmask[i] * (1 << iscale));
		ivmask[i] = (mlib_s32)(vmask[i] * (1 << iscale));
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_u16 *sa1 = sa + k;
				const mlib_s32 *hmask1 = ihmask;
				const mlib_s32 *vmask1 = ivmask;

				for (j1 = 0; j1 < 3;
				    j1++, sa1 += slb, hmask1 += 3, vmask1 += 3)
					mlib_i_ImageGradient3x3MulAdd_U16(lsh,
					    lsv, sa1, hmask1, vmask1, dw, nch);

				mlib_i_ImageGradient3x3Median_U16(da + k, lsh,
						lsv, dw, nch);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3U162S32_ext(
    mlib_s32 *dst,
    const mlib_u16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_s32 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (i = n - dx_r; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U16_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa,
	const mlib_s64 *lsa)
{
	mlib_s64 *lsh, *lsv;
	mlib_s64 *llsa = (mlib_s64 *)lsa;
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_s32 *isa = (mlib_s32 *)fsa;
	mlib_u16 *da = mlib_ImageGetData(dst);
	mlib_u16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;
	mlib_s32 ihmask[9], ivmask[9];
	mlib_s32 iscale = MLIB_S16_SCALE;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;
	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	lsh = llsa + dw + 3;
	lsv = lsh + dw;
	for (i = 0; i < dw; i++) {
		lsh[i] = MLIB_S16_SCALE_HALF;
		lsv[i] = MLIB_S16_SCALE_HALF;
	}

	for (i = 0; i < 9; i++) {
		ihmask[i] = (mlib_s32)(hmask[i] * (1 << iscale));
		ivmask[i] = (mlib_s32)(vmask[i] * (1 << iscale));
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_u16 *sa1 = sa + k;
				const mlib_s32 *hmask1 = ihmask;
				const mlib_s32 *vmask1 = ivmask;

				for (j1 = 0; j1 < 3;
				    j1++, hmask1 += 3, vmask1 += 3) {
					mlib_ImageGradient3x3U162S32_ext(isa,
					    sa1, dw + 2, nch, dx_l, dx_r);

					mlib_i_ImageGradient3x3MulAdd_S32(lsh,
					    lsv, isa, hmask1, vmask1, dw);


					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + 1 - dy_b))
						sa1 += slb;
				}

				mlib_i_ImageGradient3x3Median_U16(da + k, lsh,
				    lsv, dw, nch);

			}
		}

		if ((j >= dy_t) && (j < dh + 1 - dy_b))
			sa += slb;
	}
}

/* *********************************************************** */

#define	BUFF_SIZE	1024

/* *********************************************************** */

mlib_status
mlib_ImageGradient3x3_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_image dst_i[1], src_i[1], dst_e[1], src_e[1];
	mlib_d64 dspace[BUFF_SIZE], *dsa = dspace;
	mlib_s64 lspace[BUFF_SIZE], *lsa = lspace;
	mlib_d64 dhmask[9], dvmask[9];
	mlib_d64 offsets[2];
	mlib_s32 iscale[2];
	mlib_type type;
	mlib_s32 nchan, wid_e, bsize, dx_l, dx_r, dy_t, dy_b, i;
	mlib_s32 edg_sizes[8];
	mlib_u8 acmask[4];
	mlib_status ret;
	mlib_s32 m = 3, n = 3, dm = 1, dn = 1;

	if (dst == NULL || src == NULL || hmask == NULL || vmask == NULL) {
		return (MLIB_FAILURE);
	}

	if (hmask[0] == -1 && hmask[1] == -2 && hmask[2] == -1 &&
	    hmask[3] == 0 && hmask[4] == 0 && hmask[5] == 0 && hmask[6] == 1 &&
	    hmask[7] == 2 && hmask[8] == 1 && vmask[0] == -1 &&
	    vmask[1] == 0 && vmask[2] == 1 && vmask[3] == -2 &&
	    vmask[4] == 0 && vmask[5] == 2 && vmask[6] == -1 &&
	    vmask[7] == 0 && vmask[8] == 1) {
		return (__mlib_ImageSobel(dst, src, cmask, edge));
	}

	ret =
	    mlib_ImageClipping(dst_i, src_i, dst_e, src_e, edg_sizes, dst, src,
	    m);

	if (ret != MLIB_SUCCESS)
		return (ret);

	nchan = mlib_ImageGetChannels(dst);
	type = mlib_ImageGetType(dst);

	cmask = (nchan == 1) ? 1 : cmask;
	for (i = 0; i < nchan; i++) {
		acmask[i] = (cmask & (1 << (nchan - i - 1)));
	}

	wid_e = mlib_ImageGetWidth(src_e);
	bsize = 3 * ((wid_e + 7) & ~7) + m;

	if (bsize > BUFF_SIZE) {
		dsa = __mlib_malloc(bsize * sizeof (mlib_d64));
		lsa = __mlib_malloc(bsize * sizeof (mlib_s64));
		if (dsa == NULL)
			return (MLIB_FAILURE);
		if (lsa == NULL)
			return (MLIB_FAILURE);
	}

	if (type == MLIB_BYTE) {
		ret =
		    mlib_ImageGradientKernVIS(dhmask, dvmask, iscale, hmask,
		    vmask, offsets, type, m, n);
	}

	dx_l = edg_sizes[0];
	dx_r = edg_sizes[1];
	dy_t = edg_sizes[2];
	dy_b = edg_sizes[3];

	if (dx_l + dx_r + dy_t + dy_b == 0)
		edge = MLIB_EDGE_DST_NO_WRITE;

	if (edge != MLIB_EDGE_SRC_EXTEND) {
		if (mlib_ImageGetWidth(dst_i) >= m &&
		    mlib_ImageGetHeight(dst_i) >= n) {
/* adjust dst_i image */
			mlib_ImageSetSubimage(dst_i, dst_i, dm, dn,
			    mlib_ImageGetWidth(dst_i) - (m - 1),
			    mlib_ImageGetHeight(dst_i) - (n - 1));

			switch (type) {
			case MLIB_BYTE:

				if (ret == MLIB_SUCCESS) {
					mlib_ImageGradient3x3_U8_fast(dst_i,
					    src_i, dhmask, dvmask, iscale,
					    acmask, dsa);
				} else {
					mlib_ImageGradient3x3_U8(dst_i, src_i,
					    hmask, vmask, acmask, dsa, lsa);
				}

				break;
			case MLIB_SHORT:
				mlib_ImageGradient3x3_S16(dst_i, src_i, hmask,
				    vmask, acmask, dsa, lsa);
				break;
			case MLIB_USHORT:
				mlib_ImageGradient3x3_U16(dst_i, src_i, hmask,
				    vmask, acmask, dsa, lsa);
				break;
			case MLIB_INT:
				mlib_ImageGradient3x3_S32(dst_i, src_i, hmask,
				    vmask, acmask, dsa, lsa);
				break;
			case MLIB_FLOAT:
				mlib_ImageGradient3x3_F32(dst_i, src_i, hmask,
				    vmask, acmask, dsa);
				break;
			case MLIB_DOUBLE:
				mlib_ImageGradient3x3_D64(dst_i, src_i, hmask,
				    vmask, acmask, dsa);
				break;
			default:
				if (dsa != dspace) {
					__mlib_free(dsa);
				}
				if (lsa != lspace) {
					__mlib_free(lsa);
				}
				return (MLIB_FAILURE);
			}
		}

		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageConvZeroEdge(dst_e, dx_l, dx_r, dy_t, dy_b,
			    cmask);
			break;
		case MLIB_EDGE_DST_COPY_SRC:
			mlib_ImageConvCopyEdge(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, cmask);
			break;
		default:
			if (dsa != dspace) {
				__mlib_free(dsa);
			}
			if (lsa != lspace) {
				__mlib_free(lsa);
			}
			return (MLIB_SUCCESS);
		}
	} else if (mlib_ImageGetWidth(dst_e) > 0 &&
	    mlib_ImageGetHeight(dst_e) > 0) {
/* MLIB_EDGE_SRC_EXTEND */
/* adjust src_e image */
		mlib_ImageSetSubimage(src_e, src_e, dx_l - dm, dy_t - dn,
		    mlib_ImageGetWidth(src_e), mlib_ImageGetHeight(src_e));

		switch (type) {
		case MLIB_BYTE:

			if (ret == MLIB_SUCCESS) {
				mlib_ImageGradient3x3_U8_ext_fast(dst_e, src_e,
				    dhmask, dvmask, iscale, dx_l, dx_r, dy_t,
				    dy_b, acmask, dsa);
			} else {
				mlib_ImageGradient3x3_U8_ext(dst_e, src_e,
				    hmask, vmask, dx_l, dx_r, dy_t, dy_b,
				    acmask, dsa, lsa);
			}

			break;
		case MLIB_SHORT:
			mlib_ImageGradient3x3_S16_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa, lsa);
			break;
		case MLIB_USHORT:
			mlib_ImageGradient3x3_U16_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa, lsa);
			break;
		case MLIB_INT:
			mlib_ImageGradient3x3_S32_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa, lsa);
			break;
		case MLIB_FLOAT:
			mlib_ImageGradient3x3_F32_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa);
			break;
		case MLIB_DOUBLE:
			mlib_ImageGradient3x3_D64_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa);
			break;
		default:
			if (dsa != dspace) {
				__mlib_free(dsa);
			}
			if (lsa != lspace) {
					__mlib_free(lsa);
			}
			return (MLIB_FAILURE);
		}
	}

	if (dsa != dspace)
		__mlib_free(dsa);
	if (lsa != lspace) {
		__mlib_free(lsa);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#else	/* _NO_LONGLONG  */
/* *********************************************************** */


#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageGradient3x3 = __mlib_ImageGradient3x3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageGradient3x3) mlib_ImageGradient3x3
    __attribute__((weak, alias("__mlib_ImageGradient3x3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageGradient3x3MulAdd_U8(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_u8 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3Median_U8(
    mlib_u8 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa);

static void mlib_ImageGradient3x3U82F32_ext(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_ImageGradient3x3_U8_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa);

static void mlib_ImageGradient3x3MulAdd_S16(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_s16 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3Median_S16(
    mlib_s16 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa);

static void mlib_ImageGradient3x3MulAdd_S32(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_s32 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n);

static void mlib_ImageGradient3x3S162S32_ext(
    mlib_s32 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_ImageGradient3x3_S16_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa);

static void mlib_ImageGradient3x3MulAdd_D64(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_s32 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3Median_S32(
    mlib_s32 *dst,
    mlib_d64 *src1,
    mlib_d64 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa);

static void mlib_ImageGradient3x3S322S32_ext(
    mlib_s32 *dst,
    const mlib_s32 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_ImageGradient3x3_S32_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa);

static void mlib_ImageGradient3x3MulAdd_U16(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_u16 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3Median_U16(
    mlib_u16 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradient3x3_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa);

static void mlib_ImageGradient3x3U162S32_ext(
    mlib_s32 *dst,
    const mlib_u16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_ImageGradient3x3_U16_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa);

/* *********************************************************** */

mlib_status
__mlib_ImageGradient3x3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_type type;

	MLIB_IMAGE_CHECK(dst);
	type = mlib_ImageGetType(dst);

	if (type != MLIB_BYTE && type != MLIB_SHORT && type != MLIB_USHORT &&
	    type != MLIB_INT) {
		return (MLIB_FAILURE);
	}

	return mlib_ImageGradient3x3_alltypes(dst, src, hmask, vmask, cmask,
	    edge);
}

/* *********************************************************** */

#define	MLIB_D2_16	65536.0f

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	SAT_U8(DST, SRC)	DST = ((mlib_s32) ((SRC) * 32768.0f)) >> 15

/* *********************************************************** */

#define	SAT_S16(DST, SRC)	DST = ((mlib_s32) (SRC)) >> 16

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	DST = (((mlib_s32)((SRC) - sat)) >> 16) ^ 0x8000

/* *********************************************************** */

#define	SAT_S32(DST, SRC)	DST = (mlib_s32) (SRC)

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SAT_U8(DST, SRC)                                        \
	if ((SRC) >= MLIB_U16_MAX) {                            \
	    (SRC) = MLIB_U16_MAX;                               \
	}                                                       \
	DST = (mlib_s32)(SRC)

/* *********************************************************** */

#define	SAT_S16(DST, SRC)                                       \
	if ((SRC) > fmax_s32) {                                 \
	    (SRC) = fmax_s32;                                   \
	}                                                       \
	DST = ((mlib_s32)(SRC)) >> 16

/* *********************************************************** */

#define	SAT_U16(DST, SRC)                                       \
	(SRC) -= sat;                                           \
	if ((SRC) >= fmax_s32) {                                \
	    (SRC) = fmax_s32;                                   \
	}                                                       \
	DST = (((mlib_s32)(SRC)) >> 16) ^ 0x8000

/* *********************************************************** */

#define	SAT_S32(DST, SRC)                                       \
	if ((SRC) > fmax_s32) {                                 \
	    (SRC) = fmax_s32;                                   \
	}                                                       \
	DST = ((mlib_s32)(SRC))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

void
mlib_ImageGradient3x3MulAdd_U8(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_u8 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_f32 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_u8 *src2 = src + 2 * nch;
	mlib_f32 hval0 = (mlib_f32)hfilter[0];
	mlib_f32 vval0 = (mlib_f32)vfilter[0];
	mlib_f32 hval1 = (mlib_f32)hfilter[1];
	mlib_f32 vval1 = (mlib_f32)vfilter[1];
	mlib_f32 hval2 = (mlib_f32)hfilter[2];
	mlib_f32 vval2 = (mlib_f32)vfilter[2];
	mlib_f32 val0 = mlib_U82F32[src[0]];
	mlib_f32 val1 = mlib_U82F32[src[nch]];
	mlib_f32 hdvl = hdst[0];
	mlib_f32 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 hdvl0 = val0 * hval0 + hdvl;
		mlib_f32 vdvl0 = val0 * vval0 + vdvl;
		mlib_f32 val2 = mlib_U82F32[src2[i * nch]];

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */

#define	FAST_SQRT_U8(dp, v)                                     \
	{                                                       \
	    mlib_s32 diap;                                      \
	                                                        \
	    diap = mlib_SqrtTable_U8_Diap[(v) >> 8];            \
	    *(dp) = mlib_SqrtTable_U8[((v) >> diap) + diap];    \
	}

/* *********************************************************** */

void
mlib_ImageGradient3x3Median_U8(
    mlib_u8 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 norm = (src1[i] * src1[i] + src2[i] * src2[i]);
		mlib_s32 detl;

		SAT_U8(detl, norm);

		src1[i] = 0.5f;
		src2[i] = 0.5f;
		FAST_SQRT_U8(dst + i * nch, detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa)
{
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_u8 *da = mlib_ImageGetData(dst);
	mlib_u8 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst);
	mlib_s32 slb = mlib_ImageGetStride(src);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;

	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				const mlib_u8 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < 3;
				    j1++, sa1 += slb, hmask1 += 3, vmask1 +=
				    3) {
					mlib_ImageGradient3x3MulAdd_U8(fsh, fsv,
					    sa1, hmask1, vmask1, dw, nch);
				}

				mlib_ImageGradient3x3Median_U8(da + k, fsh, fsv,
				    dw, nch);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3U82F32_ext(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_s32 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U8_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa)
{
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_u8 *usa = (mlib_u8 *)dsa;
	mlib_u8 *da = mlib_ImageGetData(dst);
	mlib_u8 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst);
	mlib_s32 slb = mlib_ImageGetStride(src);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;

	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_u8 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < 3;
				    j1++, hmask1 += 3, vmask1 += 3) {
					mlib_ImageGradient3x3U82F32_ext(usa,
					    sa1, dw + 2, nch, dx_l, dx_r);
					mlib_ImageGradient3x3MulAdd_U8(fsh, fsv,
					    usa, hmask1, vmask1, dw, 1);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + 1 - dy_b))
						sa1 += slb;
				}

				mlib_ImageGradient3x3Median_U8(da + k, fsh, fsv,
				    dw, nch);
			}
		}

		if ((j >= dy_t) && (j < dh + 1 - dy_b))
			sa += slb;
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3MulAdd_S16(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_s16 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_f32 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_s16 *src2 = src + 2 * nch;
	mlib_f32 hval0 = (mlib_f32)hfilter[0];
	mlib_f32 vval0 = (mlib_f32)vfilter[0];
	mlib_f32 hval1 = (mlib_f32)hfilter[1];
	mlib_f32 vval1 = (mlib_f32)vfilter[1];
	mlib_f32 hval2 = (mlib_f32)hfilter[2];
	mlib_f32 vval2 = (mlib_f32)vfilter[2];
	mlib_f32 val0 = src[0];
	mlib_f32 val1 = src[nch];
	mlib_f32 hdvl = hdst[0];
	mlib_f32 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 val2 = src2[nch * i];
		mlib_f32 hdvl0 = val0 * hval0 + hdvl;
		mlib_f32 vdvl0 = val0 * vval0 + vdvl;

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */
void
mlib_ImageGradient3x3Median_S16(
    mlib_s16 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_f32 fmax_s32 = (mlib_f32)(MLIB_S32_MAX - 64);
#endif /* MLIB_USE_FTOI_CLAMPING */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 detl =
		    (mlib_sqrtf(src1[i] * src1[i] +
		    src2[i] * src2[i]) * MLIB_D2_16);
		src1[i] = 0.5f;
		src2[i] = 0.5f;
		SAT_S16(dst[i * nch], detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa)
{
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_s16 *da = mlib_ImageGetData(dst);
	mlib_s16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;

	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_s16 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < 3;
				    j1++, sa1 += slb, hmask1 += 3, vmask1 += 3)
					mlib_ImageGradient3x3MulAdd_S16(fsh,
					    fsv, sa1, hmask1, vmask1, dw, nch);

				mlib_ImageGradient3x3Median_S16(da + k, fsh,
				    fsv, dw, nch);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3MulAdd_S32(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_s32 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n)
{
	mlib_f32 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_s32 *src2 = src + 2;
	mlib_f32 hval0 = (mlib_f32)hfilter[0];
	mlib_f32 vval0 = (mlib_f32)vfilter[0];
	mlib_f32 hval1 = (mlib_f32)hfilter[1];
	mlib_f32 vval1 = (mlib_f32)vfilter[1];
	mlib_f32 hval2 = (mlib_f32)hfilter[2];
	mlib_f32 vval2 = (mlib_f32)vfilter[2];
	mlib_f32 val0 = (mlib_f32)src[0];
	mlib_f32 val1 = (mlib_f32)src[1];
	mlib_f32 hdvl = hdst[0];
	mlib_f32 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 val2 = (mlib_f32)src2[i];
		mlib_f32 hdvl0 = val0 * hval0 + hdvl;
		mlib_f32 vdvl0 = val0 * vval0 + vdvl;

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3S162S32_ext(
    mlib_s32 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_s32 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (i = n - dx_r; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_S16_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa)
{
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_s32 *isa = (mlib_s32 *)fsa;
	mlib_s16 *da = mlib_ImageGetData(dst);
	mlib_s16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;

	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_s16 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < 3;
				    j1++, hmask1 += 3, vmask1 += 3) {
					mlib_ImageGradient3x3S162S32_ext(isa,
					    sa1, dw + 2, nch, dx_l, dx_r);
					mlib_ImageGradient3x3MulAdd_S32(fsh,
					    fsv, isa, hmask1, vmask1, dw);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + 1 - dy_b))
						sa1 += slb;
				}

				mlib_ImageGradient3x3Median_S16(da + k, fsh,
				    fsv, dw, nch);
			}
		}

		if ((j >= dy_t) && (j < dh + 1 - dy_b))
			sa += slb;
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3MulAdd_D64(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_s32 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_d64 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_s32 *src2 = src + 2 * nch;
	mlib_d64 hval0 = hfilter[0];
	mlib_d64 vval0 = vfilter[0];
	mlib_d64 hval1 = hfilter[1];
	mlib_d64 vval1 = vfilter[1];
	mlib_d64 hval2 = hfilter[2];
	mlib_d64 vval2 = vfilter[2];
	mlib_d64 val0 = src[0];
	mlib_d64 val1 = src[nch];
	mlib_d64 hdvl = hdst[0];
	mlib_d64 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_d64 hdvl0 = val0 * hval0 + hdvl;
		mlib_d64 vdvl0 = val0 * vval0 + vdvl;
		mlib_d64 val2 = src2[i * nch];

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3Median_S32(
    mlib_s32 *dst,
    mlib_d64 *src1,
    mlib_d64 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_d64 fmax_s32 = (mlib_d64)(MLIB_S32_MAX);
#endif /* MLIB_USE_FTOI_CLAMPING */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_d64 detl =
		    mlib_sqrt(src1[i] * src1[i] + src2[i] * src2[i]);
		src1[i] = 0.5;
		src2[i] = 0.5;
		SAT_S32(dst[i * nch], detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa)
{
	mlib_d64 *dsh, *dsv;
	mlib_s32 *da = mlib_ImageGetData(dst);
	mlib_s32 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 2;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 2;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	dsh = (void *)(dsa + dw + 3);
	dsv = dsh + dw;

	for (i = 0; i < dw; i++) {
		dsh[i] = 0.5;
		dsv[i] = 0.5;
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_s32 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < 3;
				    j1++, sa1 += slb, hmask1 += 3, vmask1 += 3)
					mlib_ImageGradient3x3MulAdd_D64(dsh,
					    dsv, sa1, hmask1, vmask1, dw, nch);

				mlib_ImageGradient3x3Median_S32(da + k, dsh,
				    dsv, dw, nch);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3S322S32_ext(
    mlib_s32 *dst,
    const mlib_s32 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_s32 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_S32_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa)
{
	mlib_d64 *dsh, *dsv;
	mlib_s32 *isa = (mlib_s32 *)dsa;
	mlib_s32 *da = mlib_ImageGetData(dst);
	mlib_s32 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 2;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 2;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	dsh = (void *)(dsa + dw + 3);
	dsv = dsh + dw;

	for (i = 0; i < dw; i++) {
		dsh[i] = 0.5;
		dsv[i] = 0.5;
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_s32 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < 3;
				    j1++, hmask1 += 3, vmask1 += 3) {
					mlib_ImageGradient3x3S322S32_ext(isa,
					    sa1, dw + 2, nch, dx_l, dx_r);
					mlib_ImageGradient3x3MulAdd_D64(dsh,
					    dsv, isa, hmask1, vmask1, dw, 1);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + 1 - dy_b))
						sa1 += slb;
				}

				mlib_ImageGradient3x3Median_S32(da + k, dsh,
				    dsv, dw, nch);
			}
		}

		if ((j >= dy_t) && (j < dh + 1 - dy_b))
			sa += slb;
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3MulAdd_U16(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_u16 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_f32 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	const mlib_u16 *src2 = src + 2 * nch;
	mlib_f32 hval0 = (mlib_f32)hfilter[0];
	mlib_f32 vval0 = (mlib_f32)vfilter[0];
	mlib_f32 hval1 = (mlib_f32)hfilter[1];
	mlib_f32 vval1 = (mlib_f32)vfilter[1];
	mlib_f32 hval2 = (mlib_f32)hfilter[2];
	mlib_f32 vval2 = (mlib_f32)vfilter[2];
	mlib_f32 val0 = src[0];
	mlib_f32 val1 = src[nch];
	mlib_f32 hdvl = hdst[0];
	mlib_f32 vdvl = vdst[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 val2 = src2[nch * i];
		mlib_f32 hdvl0 = val0 * hval0 + hdvl;
		mlib_f32 vdvl0 = val0 * vval0 + vdvl;

		hdvl = hdst1[i];
		vdvl = vdst1[i];
		hdvl0 += val1 * hval1;
		vdvl0 += val1 * vval1;
		hdvl0 += val2 * hval2;
		vdvl0 += val2 * vval2;
		val0 = val1;
		val1 = val2;

		hdst[i] = hdvl0;
		vdst[i] = vdvl0;
	}
}

/* *********************************************************** */


void
mlib_ImageGradient3x3Median_U16(
    mlib_u16 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;
	mlib_f32 sat = (mlib_f32)(0x80000000);

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_f32 fmax_s32 = (mlib_f32)(MLIB_S32_MAX - 64);
#endif /* MLIB_USE_FTOI_CLAMPING */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 detl =
		    (mlib_sqrtf(src1[i] * src1[i] +
		    src2[i] * src2[i]) * MLIB_D2_16);
		src1[i] = 0.5f;
		src2[i] = 0.5f;
		SAT_U16(dst[i * nch], detl);
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa)
{
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_u16 *da = mlib_ImageGetData(dst);
	mlib_u16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;

	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_u16 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < 3;
				    j1++, sa1 += slb, hmask1 += 3, vmask1 += 3)
					mlib_ImageGradient3x3MulAdd_U16(fsh,
					    fsv, sa1, hmask1, vmask1, dw, nch);

				mlib_ImageGradient3x3Median_U16(da + k, fsh,
				    fsv, dw, nch);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradient3x3U162S32_ext(
    mlib_s32 *dst,
    const mlib_u16 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_s32 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (i = n - dx_r; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U16_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    const mlib_d64 *dsa)
{
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_s32 *isa = (mlib_s32 *)fsa;
	mlib_u16 *da = mlib_ImageGetData(dst);
	mlib_u16 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 1;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 1;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	fsh = fsa + dw + 3;
	fsv = fsh + dw;

	for (i = 0; i < dw; i++) {
		fsh[i] = 0.5f;
		fsv[i] = 0.5f;
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++) {
			if (acmask[k]) {
				mlib_u16 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < 3;
				    j1++, hmask1 += 3, vmask1 += 3) {
					mlib_ImageGradient3x3U162S32_ext(isa,
					    sa1, dw + 2, nch, dx_l, dx_r);
					mlib_ImageGradient3x3MulAdd_S32(fsh,
					    fsv, isa, hmask1, vmask1, dw);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + 1 - dy_b))
						sa1 += slb;
				}

				mlib_ImageGradient3x3Median_U16(da + k, fsh,
				    fsv, dw, nch);
			}
		}

		if ((j >= dy_t) && (j < dh + 1 - dy_b))
			sa += slb;
	}
}

/* *********************************************************** */

#define	BUFF_SIZE	1024

/* *********************************************************** */

mlib_status
mlib_ImageGradient3x3_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_image dst_i[1], src_i[1], dst_e[1], src_e[1];
	mlib_d64 dspace[BUFF_SIZE], *dsa = dspace;
	mlib_d64 dhmask[9], dvmask[9];
	mlib_d64 offsets[2];
	mlib_s32 iscale[2];
	mlib_type type;
	mlib_s32 nchan, wid_e, bsize, dx_l, dx_r, dy_t, dy_b, i;
	mlib_s32 edg_sizes[8];
	mlib_u8 acmask[4];
	mlib_status ret;
	mlib_s32 m = 3, n = 3, dm = 1, dn = 1;

	if (dst == NULL || src == NULL || hmask == NULL || vmask == NULL) {
		return (MLIB_FAILURE);
	}

	if (hmask[0] == -1 && hmask[1] == -2 && hmask[2] == -1 &&
	    hmask[3] == 0 && hmask[4] == 0 && hmask[5] == 0 && hmask[6] == 1 &&
	    hmask[7] == 2 && hmask[8] == 1 && vmask[0] == -1 &&
	    vmask[1] == 0 && vmask[2] == 1 && vmask[3] == -2 &&
	    vmask[4] == 0 && vmask[5] == 2 && vmask[6] == -1 &&
	    vmask[7] == 0 && vmask[8] == 1) {
		return (__mlib_ImageSobel(dst, src, cmask, edge));
	}

	ret =
	    mlib_ImageClipping(dst_i, src_i, dst_e, src_e, edg_sizes, dst, src,
	    m);

	if (ret != MLIB_SUCCESS)
		return (ret);

	nchan = mlib_ImageGetChannels(dst);
	type = mlib_ImageGetType(dst);

	cmask = (nchan == 1) ? 1 : cmask;
	for (i = 0; i < nchan; i++) {
		acmask[i] = (cmask & (1 << (nchan - i - 1)));
	}

	wid_e = mlib_ImageGetWidth(src_e);
	bsize = 3 * ((wid_e + 7) & ~7) + m;

	if (bsize > BUFF_SIZE) {
		dsa = __mlib_malloc(bsize * sizeof (mlib_d64));

		if (dsa == NULL)
			return (MLIB_FAILURE);
	}

	if (type == MLIB_BYTE) {
		ret =
		    mlib_ImageGradientKernVIS(dhmask, dvmask, iscale, hmask,
		    vmask, offsets, type, m, n);
	}

	dx_l = edg_sizes[0];
	dx_r = edg_sizes[1];
	dy_t = edg_sizes[2];
	dy_b = edg_sizes[3];

	if (dx_l + dx_r + dy_t + dy_b == 0)
		edge = MLIB_EDGE_DST_NO_WRITE;

	if (edge != MLIB_EDGE_SRC_EXTEND) {
		if (mlib_ImageGetWidth(dst_i) >= m &&
		    mlib_ImageGetHeight(dst_i) >= n) {
/* adjust dst_i image */
			mlib_ImageSetSubimage(dst_i, dst_i, dm, dn,
			    mlib_ImageGetWidth(dst_i) - (m - 1),
			    mlib_ImageGetHeight(dst_i) - (n - 1));

			switch (type) {
			case MLIB_BYTE:

				if (ret == MLIB_SUCCESS) {
					mlib_ImageGradient3x3_U8_fast(dst_i,
					    src_i, dhmask, dvmask, iscale,
					    acmask, dsa);
				} else {
					mlib_ImageGradient3x3_U8(dst_i, src_i,
					    hmask, vmask, acmask, dsa);
				}

				break;
			case MLIB_SHORT:
				mlib_ImageGradient3x3_S16(dst_i, src_i, hmask,
				    vmask, acmask, dsa);
				break;
			case MLIB_USHORT:
				mlib_ImageGradient3x3_U16(dst_i, src_i, hmask,
				    vmask, acmask, dsa);
				break;
			case MLIB_INT:
				mlib_ImageGradient3x3_S32(dst_i, src_i, hmask,
				    vmask, acmask, dsa);
				break;
			case MLIB_FLOAT:
				mlib_ImageGradient3x3_F32(dst_i, src_i, hmask,
				    vmask, acmask, dsa);
				break;
			case MLIB_DOUBLE:
				mlib_ImageGradient3x3_D64(dst_i, src_i, hmask,
				    vmask, acmask, dsa);
				break;
			default:
				if (dsa != dspace) {
					__mlib_free(dsa);
				}
				return (MLIB_FAILURE);
			}
		}

		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageConvZeroEdge(dst_e, dx_l, dx_r, dy_t, dy_b,
			    cmask);
			break;
		case MLIB_EDGE_DST_COPY_SRC:
			mlib_ImageConvCopyEdge(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, cmask);
			break;
		default:
			if (dsa != dspace) {
				__mlib_free(dsa);
			}
			return (MLIB_SUCCESS);
		}
	} else if (mlib_ImageGetWidth(dst_e) > 0 &&
	    mlib_ImageGetHeight(dst_e) > 0) {
/* MLIB_EDGE_SRC_EXTEND */
/* adjust src_e image */
		mlib_ImageSetSubimage(src_e, src_e, dx_l - dm, dy_t - dn,
		    mlib_ImageGetWidth(src_e), mlib_ImageGetHeight(src_e));

		switch (type) {
		case MLIB_BYTE:

			if (ret == MLIB_SUCCESS) {
				mlib_ImageGradient3x3_U8_ext_fast(dst_e, src_e,
				    dhmask, dvmask, iscale, dx_l, dx_r, dy_t,
				    dy_b, acmask, dsa);
			} else {
				mlib_ImageGradient3x3_U8_ext(dst_e, src_e,
				    hmask, vmask, dx_l, dx_r, dy_t, dy_b,
				    acmask, dsa);
			}

			break;
		case MLIB_SHORT:
			mlib_ImageGradient3x3_S16_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa);
			break;
		case MLIB_USHORT:
			mlib_ImageGradient3x3_U16_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa);
			break;
		case MLIB_INT:
			mlib_ImageGradient3x3_S32_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa);
			break;
		case MLIB_FLOAT:
			mlib_ImageGradient3x3_F32_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa);
			break;
		case MLIB_DOUBLE:
			mlib_ImageGradient3x3_D64_ext(dst_e, src_e, hmask,
			    vmask, dx_l, dx_r, dy_t, dy_b, acmask, dsa);
			break;
		default:
			if (dsa != dspace) {
				__mlib_free(dsa);
			}
			return (MLIB_FAILURE);
		}
	}

	if (dsa != dspace)
		__mlib_free(dsa);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#endif   /* end of _NO_LONGLONG */
/* *********************************************************** */
