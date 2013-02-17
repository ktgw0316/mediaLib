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

#pragma ident	"@(#)mlib_c_ImageFlipMainDiag_f.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageFlipMainDiag - flips image on main diag
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFlipMainDiag(mlib_image       *dst,
 *                                         const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFlipMainDiag.h>

/* *********************************************************** */

static void mlib_ImageFlipMainDiag_U8_3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

static void mlib_ImageFlipMainDiag_S16_3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

/* *********************************************************** */

mlib_status
mlib_ImageFlipRotate(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	mlib_type type;
	mlib_u8 *p_src, *p_dst;
	mlib_s32 src_stride, dst_stride;
	mlib_s32 nchan, tshift, align;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	src_stride = mlib_ImageGetStride(src);
	p_src = (void *)mlib_ImageGetData(src);

	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	dst_stride = mlib_ImageGetStride(dst);
	p_dst = (void *)mlib_ImageGetData(dst);

	if (type == MLIB_BIT) {
		if (nchan == 1) {
			return mlib_ImageFlipMainDiag_BIT_1(dst, src, x_dir,
			    y_dir);
		} else {
			return (MLIB_FAILURE);
		}
	}

	switch (type) {
	case MLIB_BYTE:
		tshift = 0;
		break;
	case MLIB_SHORT:
		tshift = 1;
		break;
	case MLIB_USHORT:
		tshift = 1;
		break;
	case MLIB_INT:
		tshift = 2;
		break;
	case MLIB_FLOAT:
		tshift = 2;
		break;
	case MLIB_DOUBLE:
		tshift = 3;
		break;
	default:
		return (MLIB_FAILURE);
	}

	align = (mlib_s32)p_src | (mlib_s32)p_dst | src_stride | dst_stride;

#ifndef i386	/* do not perform the copying by mlib_d64 data type for x86 */
	while (((nchan | (align >> tshift)) & 1) == 0 && tshift < 3) {
#else /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */
	while (((nchan | (align >> tshift)) & 1) == 0 && tshift < 2) {
#endif /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */
/* two channels as one channel of next type */
		nchan >>= 1;
		tshift++;
	}

	switch (4 * tshift + (nchan - 1)) {
	case 0:
		mlib_ImageFlipMainDiag_U8_1(dst, src, x_dir, y_dir);
		break;
	case 1:
		mlib_ImageFlipMainDiag_U8_2(dst, src, x_dir, y_dir);
		break;
	case 2:
		mlib_ImageFlipMainDiag_U8_3(dst, src, x_dir, y_dir);
		break;
	case 3:
		mlib_ImageFlipMainDiag_U8_4(dst, src, x_dir, y_dir);
		break;
	case 4:
		mlib_ImageFlipMainDiag_S16_1(dst, src, x_dir, y_dir);
		break;
	case 5:
		mlib_ImageFlipMainDiag_S16_2(dst, src, x_dir, y_dir);
		break;
	case 6:
		mlib_ImageFlipMainDiag_S16_3(dst, src, x_dir, y_dir);
		break;
	case 7:
		mlib_ImageFlipMainDiag_S16_4(dst, src, x_dir, y_dir);
		break;
	case 8:
		mlib_ImageFlipMainDiag_S32_1(dst, src, x_dir, y_dir);
		break;
	case 9:
		mlib_ImageFlipMainDiag_S32_2(dst, src, x_dir, y_dir);
		break;
	case 10:
		mlib_ImageFlipMainDiag_S32_3(dst, src, x_dir, y_dir);
		break;
	case 11:
		mlib_ImageFlipMainDiag_S32_4(dst, src, x_dir, y_dir);
		break;
	case 12:
		mlib_ImageFlipMainDiag_D64_1(dst, src, x_dir, y_dir);
		break;
	case 13:
		mlib_ImageFlipMainDiag_D64_2(dst, src, x_dir, y_dir);
		break;
	case 14:
		mlib_ImageFlipMainDiag_D64_3(dst, src, x_dir, y_dir);
		break;
	case 15:
		mlib_ImageFlipMainDiag_D64_4(dst, src, x_dir, y_dir);
		break;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 *   Old version for 3-channel images:
 *   source image by rows, destination by columns.
 */

/* *********************************************************** */

#define	dw	sh
#define	dh	sw

/* *********************************************************** */

#define	PREPAREVARS(data_type, num_ch)                                  \
	mlib_s32 sstride =                                              \
		mlib_ImageGetStride(src) / sizeof (data_type);          \
	mlib_s32 dstride =                                              \
		mlib_ImageGetStride(dst) / sizeof (data_type);          \
	mlib_s32 ws = mlib_ImageGetWidth(src);                          \
	mlib_s32 hs = mlib_ImageGetHeight(src);                         \
	mlib_s32 wd = mlib_ImageGetWidth(dst);                          \
	mlib_s32 hd = mlib_ImageGetHeight(dst);                         \
	mlib_s32 sw = (ws < hd) ? ws : hd;                              \
	mlib_s32 sh = (hs < wd) ? hs : wd;                              \
	mlib_s32 dx = ws - hd;                                          \
	mlib_s32 dy = hs - wd;                                          \
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);                  \
	mlib_s32 dyd = ((dx > 0) ? 0 : (-dx - x_dir) >> 1);             \
	mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);                  \
	mlib_s32 dxd = ((dy > 0) ? 0 : (-dy - y_dir) >> 1);             \
	data_type *sdata =                                              \
		(data_type *) mlib_ImageGetData(src) + dxs * num_ch +   \
	sstride * dys;                                                  \
	data_type *ddata =                                              \
		(data_type *) mlib_ImageGetData(dst) + dxd * num_ch +   \
	    dstride * dyd;                                              \
	if (y_dir < 0) {                                                \
	    sdata += (sh - 1) * sstride;                                \
	    sstride = -sstride;                                         \
	}                                                               \
	if (x_dir < 0) {                                                \
	    ddata += (sw - 1) * dstride;                                \
	    dstride = -dstride;                                         \
	}

/* *********************************************************** */

#define	STRIP_3(pd, ps, w, h, data_type)                        \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    data_type *_ps = (ps);                              \
	    data_type *_pd = (pd);                              \
	    data_type *spix = _ps;                              \
	    data_type *dpix = _pd;                              \
	                                                        \
	    for (x = 1; x <= _w; ++x) {                         \
		for (y = 0; y < _h; ++y) {                      \
		    data_type c1, c2, c3;                       \
	                                                        \
		    c1 = spix[0];                               \
		    c2 = spix[1];                               \
		    c3 = spix[2];                               \
		    dpix[0] = c1;                               \
		    dpix[1] = c2;                               \
		    dpix[2] = c3;                               \
		    spix += sstride;                            \
		    dpix += 3;                                  \
		}                                               \
		spix = _ps + ((x << 1) + x);                    \
		dpix = _pd + x * dstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	STRIPH_3(pd, ps, w, h, data_type)                       \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    data_type *_ps = (ps);                              \
	    data_type *_pd = (pd);                              \
	    data_type *spix = _ps;                              \
	    data_type *dpix = _pd;                              \
	                                                        \
	    for (y = 1; y <= _h; ++y) {                         \
		for (x = 0; x < _w; ++x) {                      \
		    data_type c1, c2, c3;                       \
	                                                        \
		    c1 = spix[0];                               \
		    c2 = spix[1];                               \
		    c3 = spix[2];                               \
		    dpix[0] = c1;                               \
		    dpix[1] = c2;                               \
		    dpix[2] = c3;                               \
		    dpix += dstride;                            \
		    spix += 3;                                  \
		}                                               \
		dpix = _pd + ((y << 1) + y);                    \
		spix = _ps + y * sstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	BRAK_OPEN(xpix, ypix, data_type, num_ch)                        \
	mlib_s32 s_tail = (mlib_u32)(sw - s_head) % xpix;               \
	mlib_s32 d_tail = (mlib_u32)(dw - d_head) % ypix;               \
	mlib_s32 numhorzsq = (mlib_s32)(sw - s_head - s_tail) / xpix;   \
	mlib_s32 numvertsq = (mlib_s32)(dw - d_head - d_tail) / ypix;   \
	data_type *sl = sdata + s_head * num_ch + d_head * sstride;     \
	data_type *dl = ddata + d_head * num_ch + s_head * dstride;     \
	data_type *sp0 = sl;                                            \
	data_type *dp = dl;                                             \
	mlib_s32 i, j

/* *********************************************************** */

#define	BRAK_CLOSE(xpix, ypix, data_type, num_ch)                           \
	if (d_head)                                                         \
	    STRIPH_##num_ch(ddata, sdata, (sw - s_tail), d_head,            \
		data_type);                                                 \
	if (s_head)                                                         \
	    STRIP_##num_ch(ddata + num_ch * d_head,                         \
		sdata + sstride * d_head, s_head, (dw - d_head - d_tail),   \
		data_type);                                                 \
	if (d_tail)                                                         \
	    STRIPH_##num_ch(ddata + num_ch * (dw - d_tail),                 \
		sdata + sstride * (sh - d_tail), sw, d_tail, data_type);    \
	if (s_tail)                                                         \
	    STRIP_##num_ch(ddata + dstride * (dh - s_tail),                 \
		sdata + num_ch * (sw - s_tail), s_tail, (dw - d_tail),      \
		data_type)

/* *********************************************************** */

#define	INCREMENT(xpix, ypix, num_ch)                           \
	sp0 = sl += ypix * sstride;                             \
	dp = dl += ypix * num_ch

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_U8_3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u8, 3);

	if ((sstride | dstride) & 3) {
		mlib_ImageFlipMainDiag_U8_3_na(dst, src, x_dir, y_dir);
		return;
	} else {
		mlib_s32 s_head = (mlib_addr)sdata & 3;
		mlib_s32 d_head = (mlib_addr)ddata & 3;

		BRAK_OPEN(4, 4, mlib_u8, 3);

		if (numhorzsq <= 0 || numvertsq <= 0) {
			mlib_ImageFlipMainDiag_U8_3_na(dst, src, x_dir, y_dir);
			return;
		}

		for (i = 0; i < numvertsq; i++) {
			for (j = 0; j < numhorzsq; j++) {
				mlib_u32 w0, w1, w2, w3, w4, w5, w6, w7;
				mlib_u8 *sp2 = sp0 + 2 * sstride;

				w0 = *(mlib_u32 *)sp0;
				w1 = *(mlib_u32 *)(sp0 + sstride);
				w2 = *(mlib_u32 *)(sp2);
				w3 = *(mlib_u32 *)(sp2 + sstride);

#ifdef _LITTLE_ENDIAN
				*(mlib_u32 *)dp = (w0 & 0xffffff) | (w1 << 24);
				*(mlib_u32 *)(dp + 4) =
				    ((w1 & 0xffff00) >> 8) | (w2 << 16);
				*(mlib_u32 *)(dp + 8) =
				    ((w2 & 0xff0000) >> 16) | (w3 << 8);

				w4 = *(mlib_u32 *)(sp0 + 4);
				w5 = *(mlib_u32 *)(sp0 + sstride + 4);
				w6 = *(mlib_u32 *)(sp2 + 4);
				w7 = *(mlib_u32 *)(sp2 + sstride + 4);

				dp += dstride;
				*(mlib_u32 *)(dp) =
				    w0 >> 24 | ((w4 << 8) & 0xffffff) | (w1 &
				    0xff000000);
				*(mlib_u32 *)(dp + 4) =
				    (w5 & 0xffff) | ((w2 & 0xff000000) >> 8) |
				    (w6 << 24);
				*(mlib_u32 *)(dp + 8) =
				    ((w6 >> 8) & 0xff) | ((w3 >> 16) & 0xff00) |
				    (w7 << 16);

				w0 = *(mlib_u32 *)(sp0 + 8);
				w1 = *(mlib_u32 *)(sp0 + sstride + 8);
				w2 = *(mlib_u32 *)(sp2 + 8);
				w3 = *(mlib_u32 *)(sp2 + sstride + 8);

				dp += dstride;
				*(mlib_u32 *)(dp) =
				    (w4 >> 16) | ((w0 & 0xff) << 16) | ((w5 &
				    0xff0000) << 8);
				*(mlib_u32 *)(dp + 4) =
				    (w5 >> 24) | ((w1 & 0xff) << 8) | (w6 &
				    0xffff0000);
				*(mlib_u32 *)(dp + 8) =
				    (w2 & 0xff) | ((w7 >> 8) & 0xffff00) | (w3
				    << 24);

				dp += dstride;
				*(mlib_u32 *)(dp) =
				    (w0 >> 8) | ((w1 & 0xff00) << 16);
				*(mlib_u32 *)(dp + 4) =
				    (w1 >> 16) | ((w2 & 0xffff00) << 8);
				*(mlib_u32 *)(dp + 8) =
				    (w2 >> 24) | (w3 & 0xffffff00);
#else /* _LITTLE_ENDIAN */
				*(mlib_u32 *)dp =
				    (w0 & 0xffffff00) | (w1 >> 24);
				*(mlib_u32 *)(dp + 4) =
				    ((w1 & 0xffffff00) << 8) | (w2 >> 16);
				*(mlib_u32 *)(dp + 8) =
				    ((w2 & 0xffffff00) << 16) | (w3 >> 8);

				w4 = *(mlib_u32 *)(sp0 + 4);
				w5 = *(mlib_u32 *)(sp0 + sstride + 4);
				w6 = *(mlib_u32 *)(sp2 + 4);
				w7 = *(mlib_u32 *)(sp2 + sstride + 4);

				dp += dstride;
				*(mlib_u32 *)(dp) =
				    w0 << 24 | ((w4 >> 8) & 0xffffff00) | (w1 &
				    0xff);
				*(mlib_u32 *)(dp + 4) =
				    (w5 & 0xffff0000) | ((w2 & 0xff) << 8) | (w6
				    >> 24);
				*(mlib_u32 *)(dp + 8) =
				    ((w6 & 0xffff0000) << 8) | ((w3 & 0xff) <<
				    16) | (w7 >> 16);

				w0 = *(mlib_u32 *)(sp0 + 8);
				w1 = *(mlib_u32 *)(sp0 + sstride + 8);
				w2 = *(mlib_u32 *)(sp2 + 8);
				w3 = *(mlib_u32 *)(sp2 + sstride + 8);

				dp += dstride;
				*(mlib_u32 *)(dp) =
				    (w4 << 16) | ((w0 >> 16) & 0xff00) | ((w5 >>
				    8) & 0xff);
				*(mlib_u32 *)(dp + 4) =
				    (w5 << 24) | ((w1 >> 8) & 0xff0000) | (w6 &
				    0xffff);
				*(mlib_u32 *)(dp + 8) =
				    (w2 & 0xff000000) | ((w7 & 0xffff) << 8) |
				    (w3 >> 24);

				dp += dstride;
				*(mlib_u32 *)(dp) =
				    (w0 << 8) | ((w1 >> 16) & 0xff);
				*(mlib_u32 *)(dp + 4) =
				    (w1 << 16) | ((w2 >> 8) & 0xffff);
				*(mlib_u32 *)(dp + 8) =
				    (w2 << 24) | (w3 & 0xffffff);
#endif /* _LITTLE_ENDIAN */
				sp0 += 12;
				dp += dstride;
			}

			INCREMENT(4, 4, 3);
		}

		BRAK_CLOSE(4, 4, mlib_u8, 3);
	}
}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_S16_3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u16, 3);

	if ((sstride | dstride) & 1) {
		mlib_ImageFlipMainDiag_S16_3_na(dst, src, x_dir, y_dir);
		return;
	} else {
		mlib_s32 s_head = ((mlib_addr)sdata & 2) >> 1;
		mlib_s32 d_head = ((mlib_addr)ddata & 2) >> 1;

		BRAK_OPEN(2, 2, mlib_u16, 3);

		for (i = 0; i < numvertsq; i++) {
			for (j = 0; j < numhorzsq; j++) {
				mlib_u32 w0, w1, w2, w3, w4, w5;
				mlib_u16 *sp2 = sp0 + sstride;

				w0 = *(mlib_u32 *)sp0;
				w1 = *(mlib_u32 *)(sp0 + 2);
				w2 = *(mlib_u32 *)(sp0 + 4);
				w3 = *(mlib_u32 *)sp2;
				w4 = *(mlib_u32 *)(sp2 + 2);
				w5 = *(mlib_u32 *)(sp2 + 4);

#ifdef _LITTLE_ENDIAN
				*(mlib_u32 *)dp = w0;
				*(mlib_u32 *)(dp + 2) =
				    (w1 & 0xffff) | (w3 << 16);
				*(mlib_u32 *)(dp + 4) = (w3 >> 16) | (w4 << 16);
				dp += dstride;

				*(mlib_u32 *)dp = (w1 >> 16) | (w2 << 16);
				*(mlib_u32 *)(dp + 2) =
				    (w2 >> 16) | (w4 & 0xffff0000);
				*(mlib_u32 *)(dp + 4) = w5;
#else /* _LITTLE_ENDIAN */
				*(mlib_u32 *)dp = w0;
				*(mlib_u32 *)(dp + 2) =
				    (w1 & 0xffff0000) | (w3 >> 16);
				*(mlib_u32 *)(dp + 4) = (w3 << 16) | (w4 >> 16);
				dp += dstride;

				*(mlib_u32 *)dp = (w1 << 16) | (w2 >> 16);
				*(mlib_u32 *)(dp + 2) =
				    (w2 << 16) | (w4 & 0x0000ffff);
				*(mlib_u32 *)(dp + 4) = w5;
#endif /* _LITTLE_ENDIAN */
				sp0 += 6;
				dp += dstride;
			}

			INCREMENT(2, 2, 3);
		}

		BRAK_CLOSE(2, 2, mlib_u16, 3);
	}
}

/* *********************************************************** */
