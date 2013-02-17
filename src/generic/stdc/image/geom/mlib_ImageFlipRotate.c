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

#pragma ident	"@(#)mlib_ImageFlipRotate.c	9.2	07/10/09 SMI"

/*
 */

#include <mlib_image.h>

/* *********************************************************** */

#define	dw	sh
#define	dh	sw

#define	PREPAREVARS(data_type, num_ch)                                  \
	mlib_s32 sstride =                                              \
	    mlib_ImageGetStride(src) / sizeof (data_type);              \
	mlib_s32 dstride =                                              \
	    mlib_ImageGetStride(dst) / sizeof (data_type);              \
	mlib_s32 ws = mlib_ImageGetWidth(src);                          \
	mlib_s32 hs = mlib_ImageGetHeight(src);                         \
	mlib_s32 wd = mlib_ImageGetWidth(dst);                          \
	mlib_s32 hd = mlib_ImageGetHeight(dst);                         \
	mlib_s32 sw = (ws < hd) ? ws : hd;                              \
	mlib_s32 sh = (hs < wd) ? hs : wd;                              \
	mlib_s32 dx = (mlib_s32)ws - (mlib_s32)hd;                      \
	mlib_s32 dy = (mlib_s32)hs - (mlib_s32)wd;                      \
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);                  \
	mlib_s32 dyd = ((dx > 0) ? 0 : (-dx - x_dir) >> 1);             \
	mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);                  \
	mlib_s32 dxd = ((dy > 0) ? 0 : (-dy - y_dir) >> 1);             \
	data_type *sdata =                                              \
	    (data_type *) mlib_ImageGetData(src) + dxs * num_ch +       \
	sstride * dys;                                                  \
	data_type *ddata =                                              \
	    (data_type *) mlib_ImageGetData(dst) + dxd * num_ch +       \
	dstride * dyd;                                                  \
	data_type *sp, *dp;                                             \
	mlib_s32 i, j;                                                  \
	                                                                \
	if (y_dir < 0) {                                                \
	    sdata += (sh - 1) * sstride;                                \
	    sstride = -sstride;                                         \
	}                                                               \
	if (x_dir < 0) {                                                \
	    ddata += (sw - 1) * dstride;                                \
	    dstride = -dstride;                                         \
	}                                                               \
	sp = sdata;                                                     \
	dp = ddata

/* *********************************************************** */

#define	BRAK_OPEN(xpix, ypix, data_type, num_ch)                          \
	mlib_s32 s_tail = (mlib_addr)(sw - s_head) % xpix;                \
	mlib_s32 d_tail = (mlib_addr)(dw - d_head) % ypix;                \
	mlib_s32 numhorzsq = (mlib_s32)(sw - s_head - s_tail) / xpix;     \
	mlib_s32 numvertsq = (mlib_s32)(dw - d_head - d_tail) / ypix;     \
	data_type *pssq_a = sdata + s_head * num_ch + d_head * sstride;   \
	data_type *pdsq_a = ddata + d_head * num_ch + s_head * dstride;   \
	data_type *pssq = pssq_a;                                         \
	data_type *pdsq = pdsq_a;                                         \
	mlib_s32 ysq;

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
		data_type);

/* *********************************************************** */

#define	INCREMENT(xpix, ypix, num_ch)                           \
	pssq = pssq_a + ysq * ypix * sstride;                   \
	pdsq = pdsq_a + ysq * (ypix * num_ch);

/* *********************************************************** */

#define	STRIP_2(pd, ps, w, h, data_type)                        \
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
		    data_type c1, c2;                           \
	                                                        \
		    c1 = spix[0];                               \
		    c2 = spix[1];                               \
		    dpix[0] = c1;                               \
		    dpix[1] = c2;                               \
		    spix += sstride;                            \
		    dpix += 2;                                  \
		}                                               \
		spix = _ps + (x << 1);                          \
		dpix = _pd + x * dstride;                       \
	    }                                                   \
	}

/* *********************************************************** */
#define	STRIPH_2(pd, ps, w, h, data_type)                       \
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
		    data_type c1, c2;                           \
	                                                        \
		    c1 = spix[0];                               \
		    c2 = spix[1];                               \
		    dpix[0] = c1;                               \
		    dpix[1] = c2;                               \
		    dpix += dstride;                            \
		    spix += 2;                                  \
		}                                               \
		dpix = _pd + (y << 1);                          \
		spix = _ps + y * sstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	DIRECT_TRANSP_4X4_D64_S32                               \
	{                                                       \
	    mlib_d64 a0, a1, a2, a3;                            \
	    mlib_d64 b0, b1, b2, b3;                            \
	    const mlib_u32 *psq2 = pssq + 2 * sstride;          \
	                                                        \
	    a0 = *(mlib_d64 *)pssq;                             \
	    a1 = *(mlib_d64 *)(pssq + sstride);                 \
	    a2 = *(mlib_d64 *)psq2;                             \
	    a3 = *(mlib_d64 *)(psq2 + sstride);                 \
	    pssq += 2;                                          \
	    psq2 += 2;                                          \
	    b0 = *(mlib_d64 *)pssq;                             \
	    b1 = *(mlib_d64 *)(pssq + sstride);                 \
	    b2 = *(mlib_d64 *)psq2;                             \
	    b3 = *(mlib_d64 *)(psq2 + sstride);                 \
	    pssq += 2;                                          \
	    psq2 += 2;                                          \
	    *(mlib_d64 *)pdsq = a0;                             \
	    *(mlib_d64 *)(pdsq + 2) = a1;                       \
	    *(mlib_d64 *)(pdsq + 4) = a2;                       \
	    *(mlib_d64 *)(pdsq + 6) = a3;                       \
	    pdsq += dstride;                                    \
	    a0 = *(mlib_d64 *)pssq;                             \
	    a1 = *(mlib_d64 *)(pssq + sstride);                 \
	    a2 = *(mlib_d64 *)psq2;                             \
	    a3 = *(mlib_d64 *)(psq2 + sstride);                 \
	    pssq += 2;                                          \
	    psq2 += 2;                                          \
	    *(mlib_d64 *)pdsq = b0;                             \
	    *(mlib_d64 *)(pdsq + 2) = b1;                       \
	    *(mlib_d64 *)(pdsq + 4) = b2;                       \
	    *(mlib_d64 *)(pdsq + 6) = b3;                       \
	    pdsq += dstride;                                    \
	    b0 = *(mlib_d64 *)pssq;                             \
	    b1 = *(mlib_d64 *)(pssq + sstride);                 \
	    b2 = *(mlib_d64 *)psq2;                             \
	    b3 = *(mlib_d64 *)(psq2 + sstride);                 \
	    pssq += 2;                                          \
	    *(mlib_d64 *)pdsq = a0;                             \
	    *(mlib_d64 *)(pdsq + 2) = a1;                       \
	    *(mlib_d64 *)(pdsq + 4) = a2;                       \
	    *(mlib_d64 *)(pdsq + 6) = a3;                       \
	    pdsq += dstride;                                    \
	    *(mlib_d64 *)pdsq = b0;                             \
	    *(mlib_d64 *)(pdsq + 2) = b1;                       \
	    *(mlib_d64 *)(pdsq + 4) = b2;                       \
	    *(mlib_d64 *)(pdsq + 6) = b3;                       \
	    pdsq += dstride;                                    \
	}

/* *********************************************************** */

void
mlib_ImageFlipMainDiag_D64_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	PREPAREVARS(mlib_u32,
	    2);

	if (sw < 2 || sh < 2) {
		STRIP_2(ddata, sdata, sw, sh, mlib_u32);

		return;
	}

	{
		const int s_head = 0;
		const int d_head = 0;

		BRAK_OPEN(4, 4, mlib_u32, 2)

		for (
		    ysq = 1; ysq <= numvertsq; ++ysq) {
			int xsq;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#endif /* __SUNPRO_C */
			for (xsq = 0; xsq < numhorzsq; ++xsq) {
				DIRECT_TRANSP_4X4_D64_S32;
			}
			INCREMENT(4, 4, 2);
		}
		BRAK_CLOSE(4, 4, mlib_u32, 2)
	}
}

/* *********************************************************** */
