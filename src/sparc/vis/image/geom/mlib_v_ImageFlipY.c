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

#pragma ident	"@(#)mlib_v_ImageFlipY.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageFlipY - flip image on Y axis
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFlipY(mlib_image       *dst,
 *                                  const mlib_image *src)
 *
 * ARGUMENTS
 *      dst     pointer to an dst image
 *      src     pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFlipY.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageFlipY = __mlib_ImageFlipY

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageFlipY) mlib_ImageFlipY
    __attribute__((weak, alias("__mlib_ImageFlipY")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_v_ImageFlipY_U8_1(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageFlipY_S16_1(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageFlipY_S32_1(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageFlipY_U8_2(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageFlipY_U8_3(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageFlipY_S16_3(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageFlipY_S32_3(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageFlipY_S16_4(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageFlipY(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;
	mlib_s32 nchan, tshift;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	if (type == MLIB_BIT) {
		if (nchan != 1)
			return (MLIB_FAILURE);
		mlib_ImageFlipY_BIT_1(dst, src);
		return (MLIB_SUCCESS);
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
	default:
		return (MLIB_FAILURE);
	}

	if ((nchan & 1) == 0 && tshift <= 1) {
		mlib_u8 *p_src, *p_dst;
		mlib_s32 src_stride, dst_stride, align;

		type = mlib_ImageGetType(src);
		nchan = mlib_ImageGetChannels(src);
		src_stride = mlib_ImageGetStride(src);
		p_src = (void *)mlib_ImageGetData(src);
		type = mlib_ImageGetType(dst);
		nchan = mlib_ImageGetChannels(dst);
		dst_stride = mlib_ImageGetStride(dst);
		p_dst = (void *)mlib_ImageGetData(dst);

		align =
		    (mlib_s32)p_src | (mlib_s32)p_dst | src_stride | dst_stride;

		while (((nchan | (align >> tshift)) & 1) == 0 && tshift <= 1) {
/* two channels as one channel of next type */
			nchan >>= 1;
			tshift++;
		}
	}

	switch (4 * tshift + (nchan - 1)) {
	case 0:
		mlib_v_ImageFlipY_U8_1(dst, src);
		break;
	case 1:
		mlib_v_ImageFlipY_U8_2(dst, src);
		break;
	case 2:
		mlib_v_ImageFlipY_U8_3(dst, src);
		break;
	case 3:
		mlib_ImageFlipY_U8_4(dst, src);
		break;
	case 4:
		mlib_v_ImageFlipY_S16_1(dst, src);
		break;
	case 5:
		mlib_ImageFlipY_S16_2(dst, src);
		break;
	case 6:
		mlib_v_ImageFlipY_S16_3(dst, src);
		break;
	case 7:
		mlib_ImageFlipY_S16_4(dst, src);
		break;
	case 8:
		mlib_v_ImageFlipY_S32_1(dst, src);
		break;
	case 9:
		mlib_ImageFlipY_S32_2(dst, src);
		break;
	case 10:
		mlib_v_ImageFlipY_S32_3(dst, src);
		break;
	case 11:
		mlib_ImageFlipY_S32_4(dst, src);
		break;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PREPAREVARS(data_type, num_ch)                                     \
	mlib_s32 sstride =                                                 \
	    mlib_ImageGetStride(src) / (mlib_s32)sizeof (data_type);       \
	mlib_s32 dstride =                                                 \
		mlib_ImageGetStride(dst) / (mlib_s32)sizeof (data_type);   \
	mlib_s32 ws = mlib_ImageGetWidth(src);                             \
	mlib_s32 hs = mlib_ImageGetHeight(src);                            \
	mlib_s32 wd = mlib_ImageGetWidth(dst);                             \
	mlib_s32 hd = mlib_ImageGetHeight(dst);                            \
	mlib_s32 sw = (ws < wd) ? ws : wd;                                 \
	mlib_s32 sh = (hs < hd) ? hs : hd;                                 \
	mlib_s32 dx = ((mlib_s32)ws - (mlib_s32)wd);                       \
	mlib_s32 dy = ((mlib_s32)hs - (mlib_s32)hd);                       \
	mlib_s32 Q = (sstride >= 0) ? 1 : 0;                               \
/*                                                                         \
 * for Rotate180                                                           \
 */                                                                        \
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);                     \
	mlib_s32 dxd = ((dx > 0) ? 0 : (1 - dx) >> 1);                     \
	mlib_s32 dys = ((dy > 0) ? (dy + Q) >> 1 : 0);                     \
	mlib_s32 dyd = ((dy > 0) ? 0 : (1 - Q - dy) >> 1);                 \
	data_type *sdata =                                                 \
		(data_type *) mlib_ImageGetData(src) + dxs * num_ch +      \
	sstride *dys;                                                      \
	data_type *ddata =                                                 \
		(data_type *) mlib_ImageGetData(dst) + dxd * num_ch +      \
	dstride *dyd

/* *********************************************************** */

#define	STRIP_1(pd, ps, w, h, data_type)                              \
	{                                                             \
	    mlib_s32 _w = (w), _h = (h);                              \
	    data_type *_ps = (ps), *_pd = (pd);                       \
	    data_type s0, s1;                                         \
	                                                              \
	    for (i = 0; i < _h; i++) {                                \
		if (j = _w & 1)                                       \
		    _pd[i * dstride + (_w - 1)] = _ps[i * sstride];   \
		for (; j < _w; j += 2) {                              \
		    s0 = _ps[i * sstride + j];                        \
		    s1 = _ps[i * sstride + j + 1];                    \
		    _pd[i * dstride + (_w - j - 1)] = s0;             \
		    _pd[i * dstride + (_w - j - 2)] = s1;             \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	CHANGE_S16_1                                                \
	ddst = vis_fpmerge(vis_read_lo(dsrc), vis_read_hi(dsrc));   \
	ddst = vis_fpmerge(vis_read_lo(ddst), vis_read_hi(ddst));   \
	ddst = vis_fpmerge(vis_read_hi(ddst), vis_read_lo(ddst))

/* *********************************************************** */

#define	CHANGE_S32_1                                                 \
	ddst = vis_freg_pair(vis_read_lo(dsrc), vis_read_hi(dsrc))

/* *********************************************************** */

void
mlib_v_ImageFlipY_U8_1(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 even_8;
	mlib_d64 *dpsrc, *dpdst;
	mlib_d64 dsrc0, dsrc1, ddst;
	mlib_s32 i, j;
	mlib_s32 d_head, d_tail;
	mlib_u8 *psrc, *pdst;
	mlib_s32 dist;

	PREPAREVARS(mlib_u8,
	    1);

	if (sw < 8) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u8);
		return;
	}

	if (((sstride | dstride) & 7) == 0) {
		d_head = (8 - (mlib_addr)ddata) & 7;
		d_tail = (sw - d_head) & 7;
		dist = sw - d_head - d_tail;
		even_8 = dist >> 3;

		if (d_head)
			STRIP_1(ddata, sdata + (sw - d_head), d_head, sh,
			    mlib_u8);

		if (d_tail)
			STRIP_1(ddata + (sw - d_tail), sdata, d_tail, sh,
			    mlib_u8);
		psrc = sdata + d_tail;
		pdst = ddata + sw - d_tail - 8;

		if (((mlib_addr)psrc & 7) != 0) {
			for (i = 0; i < sh; i++) {
				dpsrc = (mlib_d64 *)vis_alignaddr(psrc,
				    8 - (((mlib_addr)psrc & 7) << 1));
				dsrc0 = vis_ld_d64_le(dpsrc);
				dsrc1 = vis_ld_d64_nf_le(dpsrc + 1);
				dpdst = (mlib_d64 *)pdst;
				ddst = vis_faligndata(dsrc1, dsrc0);

				for (j = 0; j < even_8; j++) {
					dsrc0 = dsrc1;
/* dpsrc[2]; */
					dsrc1 = vis_ld_d64_nf_le(dpsrc + 2);
					dpdst[0] = ddst;
					ddst = vis_faligndata(dsrc1, dsrc0);
					dpdst--;
					dpsrc++;
				}

				psrc += sstride;
				pdst += dstride;
			}
		} else {
			for (i = 0; i < sh; i++) {
				dpsrc = (mlib_d64 *)psrc;
				dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
				for (j = 0; j < even_8; j++) {
					dpdst[0] = vis_ld_d64_le(dpsrc);
					dpdst--;
					dpsrc++;
				}

				psrc += sstride;
				pdst += dstride;
			}
		}
	} else {
		for (i = 0; i < sh; i++) {
			psrc = sdata + i * sstride;
			pdst = ddata + i * dstride + sw;
			d_head = (mlib_addr)pdst & 7;
			d_tail = (sw - d_head) & 7;
			even_8 = (sw - d_head) >> 3;

			for (j = 0; j < d_head; j++) {
				pdst[-j - 1] = psrc[j];
			}

			psrc += d_head;
			pdst -= d_head;

			if (((mlib_addr)psrc & 7) != 0) {

				dpsrc = (mlib_d64 *)vis_alignaddr(psrc,
				    8 - (((mlib_addr)psrc & 7) << 1));
				dsrc0 = vis_ld_d64_le(dpsrc);
				dsrc1 = vis_ld_d64_nf_le(dpsrc + 1);
				ddst = vis_faligndata(dsrc1, dsrc0);

				for (j = 0; j < even_8; j++) {
					dsrc0 = dsrc1;
/* dpsrc[2]; */
					dsrc1 = vis_ld_d64_nf_le(dpsrc + 2);
					pdst -= 8;
					((mlib_d64 *)pdst)[0] = ddst;
					ddst = vis_faligndata(dsrc1, dsrc0);
					dpsrc++;
				}

			} else {
				dpsrc = (mlib_d64 *)psrc;

#pragma pipeloop(0)
				for (j = 0; j < even_8; j++) {
					pdst -= 8;
					ddst = vis_ld_d64_le(dpsrc);
					((mlib_d64 *)pdst)[0] = ddst;
					dpsrc++;
				}
			}

			psrc += even_8 * 8;

			for (j = 0; j < d_tail; j++) {
				pdst[-j - 1] = psrc[j];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageFlipY_S16_1(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 even_4;
	mlib_d64 *dpsrc, *dpdst;
	mlib_d64 dsrc0, dsrc1, dsrc, ddst;
	mlib_s32 i, j;
	mlib_s32 d_head, d_tail;
	mlib_u16 *psrc, *pdst;
	mlib_s32 dist;

	PREPAREVARS(mlib_u16, 1);

	if (sw < 4) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u16);
		return;
	}

	if (((sstride | dstride) & 3) == 0) {
		d_head = ((8 - (mlib_addr)ddata) & 7) >> 1;
		d_tail = (sw - d_head) & 3;
		dist = sw - d_head - d_tail;
		even_4 = dist >> 2;

		if (d_head)
			STRIP_1(ddata, sdata + (sw - d_head), d_head, sh,
			    mlib_u16);

		if (d_tail)
			STRIP_1(ddata + (sw - d_tail), sdata, d_tail, sh,
			    mlib_u16);
		psrc = sdata + d_tail;
		pdst = ddata + sw - d_tail - 4;

		if (((mlib_addr)psrc & 7) != 0) {
			for (i = 0; i < sh; i++) {
				dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
				dsrc0 = dpsrc[0];
				dsrc1 = vis_ld_d64_nf(dpsrc + 1);
				dpdst = (mlib_d64 *)pdst;
				dsrc = vis_faligndata(dsrc0, dsrc1);
#pragma pipeloop(0)
				for (j = 0; j < even_4; j++) {
					dsrc0 = dsrc1;
					dsrc1 = vis_ld_d64_nf(dpsrc + 2);
					CHANGE_S16_1;
					dpdst[0] = ddst;
					dsrc = vis_faligndata(dsrc0, dsrc1);
					dpdst--;
					dpsrc++;
				}

				psrc += sstride;
				pdst += dstride;
			}
		} else {
			for (i = 0; i < sh; i++) {
				dpsrc = (mlib_d64 *)psrc;
				dsrc = dpsrc[0];
				dpdst = (mlib_d64 *)pdst;
#pragma pipeloop(0)
				for (j = 0; j < even_4; j++) {
					dsrc1 = vis_ld_d64_nf(dpsrc + 1);
					CHANGE_S16_1;
					dpdst[0] = ddst;
					dsrc = dsrc1;
					dpdst--;
					dpsrc++;
				}

				psrc += sstride;
				pdst += dstride;
			}
		}

	} else {
		mlib_ImageFlipY_S16_1(dst, src);
	}
}

/* *********************************************************** */

void
mlib_v_ImageFlipY_S32_1(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 even_2;
	mlib_d64 *dpsrc, *dpdst;
	mlib_d64 dsrc0, dsrc1, dsrc, ddst;
	mlib_s32 i, j;
	mlib_s32 d_head, d_tail;
	mlib_u32 *psrc, *pdst;
	mlib_s32 dist;

	PREPAREVARS(mlib_u32, 1);

	if (sw < 2) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u32);
		return;
	}

	if (((sstride | dstride) & 1) == 0) {
		d_head = ((8 - (mlib_addr)ddata) & 7) >> 2;
		d_tail = (sw - d_head) & 1;
		dist = sw - d_head - d_tail;
		even_2 = dist >> 1;

		if (d_head)
			STRIP_1(ddata, sdata + (sw - d_head), d_head, sh,
			    mlib_u32);

		if (d_tail)
			STRIP_1(ddata + (sw - d_tail), sdata, d_tail, sh,
			    mlib_u32);
		psrc = sdata + d_tail;
		pdst = ddata + sw - d_tail - 2;

		if (((mlib_addr)psrc & 7) != 0) {
			for (i = 0; i < sh; i++) {
				dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
				dsrc0 = dpsrc[0];
				dsrc1 = vis_ld_d64_nf(dpsrc + 1);
				dpdst = (mlib_d64 *)pdst;
				dsrc = vis_faligndata(dsrc0, dsrc1);
#pragma pipeloop(0)
				for (j = 0; j < even_2; j++) {
					dsrc0 = dsrc1;
					dsrc1 = vis_ld_d64_nf(dpsrc + 2);
					CHANGE_S32_1;
					dpdst[0] = ddst;
					dsrc = vis_faligndata(dsrc0, dsrc1);
					dpdst--;
					dpsrc++;
				}

				psrc += sstride;
				pdst += dstride;
			}
		} else {
			for (i = 0; i < sh; i++) {
				dpsrc = (mlib_d64 *)psrc;
				dsrc = dpsrc[0];
				dpdst = (mlib_d64 *)pdst;
#pragma pipeloop(0)
				for (j = 0; j < even_2; j++) {
					dsrc1 = vis_ld_d64_nf(dpsrc + 1);
					CHANGE_S32_1;
					dpdst[0] = ddst;
					dsrc = dsrc1;
					dpdst--;
					dpsrc++;
				}

				psrc += sstride;
				pdst += dstride;
			}
		}
	} else {
		for (i = 0; i < sh; i++) {
			psrc = sdata + i * sstride;
			pdst = ddata + i * dstride + sw;
			d_head = ((mlib_addr)pdst & 7) >> 2;
			d_tail = (sw - d_head) & 1;
			even_2 = (sw - d_head) >> 1;

			for (j = 0; j < d_head; j++) {
				pdst[-j - 1] = psrc[j];
			}

			psrc += d_head;
			pdst -= d_head;
			dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
			dsrc0 = dpsrc[0];
			dsrc1 = vis_ld_d64_nf(dpsrc + 1);
			dsrc = vis_faligndata(dsrc0, dsrc1);

#pragma pipeloop(0)
			for (j = 0; j < even_2; j++) {
				dsrc0 = dsrc1;
				dsrc1 = vis_ld_d64_nf(dpsrc + 2);
				CHANGE_S32_1;
				pdst -= 2;
				((mlib_d64 *)pdst)[0] = ddst;
				dsrc = vis_faligndata(dsrc0, dsrc1);
				dpsrc++;
			}

			psrc += even_2 * 2;

			for (j = 0; j < d_tail; j++) {
				pdst[-j - 1] = psrc[j];
			}
		}
	}
}

/* *********************************************************** */

#define	STRIP_2(pd, ps, w, h, data_type)                            \
	{                                                           \
	    mlib_s32 i, j, _w = (w), _h = (h);                      \
	    data_type *_ps = (ps), *_pd = (pd);                     \
	    data_type s0, s1;                                       \
	                                                            \
	    for (i = 0; i < _h; i++) {                              \
		for (j = 0; j < _w; j++) {                          \
		    s0 = _ps[i * sstride + 2 * j];                  \
		    s1 = _ps[i * sstride + 2 * j + 1];              \
		    _pd[i * dstride + 2 * (_w - j - 1)] = s0;       \
		    _pd[i * dstride + 2 * (_w - j - 1) + 1] = s1;   \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	NASTORE_U8                                              \
	vis_alignaddr((void *)all_dst, 1);                      \
	ddst = vis_faligndata(ddst, ddst);                      \
	vis_pst_8(ddst, dpdst, emask_lo);                       \
	vis_pst_8(ddst, dpdst - 1, emask_hi)

/* *********************************************************** */

void
mlib_v_ImageFlipY_U8_2(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 even_4;
	mlib_d64 *dpsrc, *dpdst;
	mlib_d64 dsrc0, dsrc1, dsrc, ddst;
	mlib_s32 i, j;
	mlib_s32 d_head, d_tail;
	mlib_u8 *psrc, *pdst;
	mlib_s32 emask_hi, emask_lo;
	mlib_s32 all_dst;

	PREPAREVARS(mlib_u8, 2);

	if (sw < 4) {
		STRIP_2(ddata, sdata, sw, sh, mlib_u8);
		return;
	}

	if ((((mlib_addr)ddata | dstride) & 1) == 0) {
		for (i = 0; i < sh; i++) {
			psrc = sdata + i * sstride;
			pdst = ddata + i * dstride + 2 * sw;
			d_head = ((mlib_addr)pdst & 7) >> 1;
			d_tail = (sw - d_head) & 3;
			even_4 = (sw - d_head) >> 2;

			for (j = 0; j < d_head; j++) {
				pdst[2 * (-j - 1)] = psrc[2 * j];
				pdst[2 * (-j - 1) + 1] = psrc[2 * j + 1];
			}

			psrc += 2 * d_head;
			pdst -= 2 * d_head;
			dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
			dsrc0 = dpsrc[0];
			dsrc1 = vis_ld_d64_nf(dpsrc + 1);
			dsrc = vis_faligndata(dsrc0, dsrc1);

#pragma pipeloop(0)
			for (j = 0; j < even_4; j++) {
				dsrc0 = dsrc1;
				dsrc1 = vis_ld_d64_nf(dpsrc + 2);
				CHANGE_S16_1;
				pdst -= 8;
				((mlib_d64 *)pdst)[0] = ddst;
				dsrc = vis_faligndata(dsrc0, dsrc1);
				dpsrc++;
			}

			psrc += even_4 * 8;

			for (j = 0; j < d_tail; j++) {
				pdst[2 * (-j - 1)] = psrc[2 * j];
				pdst[2 * (-j - 1) + 1] = psrc[2 * j + 1];
			}
		}

	} else {
		for (i = 0; i < sh; i++) {
			psrc = sdata + i * sstride;
			pdst = ddata + i * dstride + 2 * sw;
			d_tail = sw & 3;
			even_4 = sw >> 2;

			dpdst = (mlib_d64 *)vis_alignaddr(pdst, 8);
			emask_hi = vis_edge8(pdst, dpdst);
			emask_lo = ~emask_hi;
			all_dst = ~(mlib_addr)pdst;

			dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
			dsrc0 = dpsrc[0];
			dsrc1 = vis_ld_d64_nf(dpsrc + 1);
			dsrc = vis_faligndata(dsrc0, dsrc1);
			CHANGE_S16_1;

			if ((mlib_addr)pdst & 7) {
#pragma pipeloop(0)
				for (j = 0; j < even_4; j++) {
					dsrc0 = dsrc1;
					dsrc1 = vis_ld_d64_nf(dpsrc + 2);
					dpdst--;
					NASTORE_U8;
					(void *)vis_alignaddr(psrc, 0);
					dsrc = vis_faligndata(dsrc0, dsrc1);
					CHANGE_S16_1;
					dpsrc++;
				}
			} else {
				dpdst--;
#pragma pipeloop(0)
				for (j = 0; j < even_4; j++) {
					dsrc0 = dsrc1;
					dsrc1 = vis_ld_d64_nf(dpsrc + 2);
					dpdst--;
					dpdst[0] = ddst;
					dsrc = vis_faligndata(dsrc0, dsrc1);
					CHANGE_S16_1;
					dpsrc++;
				}
			}

			psrc += even_4 * 8;
			pdst -= even_4 * 8;

			for (j = 0; j < d_tail; j++) {
				pdst[2 * (-j - 1)] = psrc[2 * j];
				pdst[2 * (-j - 1) + 1] = psrc[2 * j + 1];
			}
		}
	}
}

/* *********************************************************** */

#define	STRIP_3(pd, ps, w, h, data_type)                            \
	{                                                           \
	    mlib_s32 i, j, _w = (w), _h = (h);                      \
	    data_type *_ps = (ps), *_pd = (pd);                     \
	    data_type s0, s1, s2;                                   \
	                                                            \
	    for (i = 0; i < _h; i++) {                              \
		for (j = 0; j < _w; j++) {                          \
		    s0 = _ps[i * sstride + 3 * j];                  \
		    s1 = _ps[i * sstride + 3 * j + 1];              \
		    s2 = _ps[i * sstride + 3 * j + 2];              \
		    _pd[i * dstride + 3 * (_w - j - 1)] = s0;       \
		    _pd[i * dstride + 3 * (_w - j - 1) + 1] = s1;   \
		    _pd[i * dstride + 3 * (_w - j - 1) + 2] = s2;   \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	CHANGE_U8_3                                             \
	(void *)vis_alignaddr(dpsrc, 2);                        \
	ddst3 = vis_faligndata(dsrc2, dsrc2);                   \
	(void *)vis_alignaddr(dpsrc, 4);                        \
	ddst1 = vis_faligndata(dsrc1, dsrc1);                   \
	(void *)vis_alignaddr(dpsrc, 3);                        \
	ddst0 = vis_faligndata(dsrc2, ddst3);                   \
	ddst5 = vis_faligndata(ddst3, ddst1);                   \
	(void *)vis_alignaddr(dpsrc, 7);                        \
	ddst4 = vis_faligndata(dsrc1, dsrc2);                   \
	(void *)vis_alignaddr(dpsrc, 2);                        \
	ddst0 = vis_faligndata(ddst0, ddst4);                   \
	(void *)vis_alignaddr(dpsrc, 6);                        \
	ddst6 = vis_faligndata(dsrc0, dsrc0);                   \
	(void *)vis_alignaddr(dpsrc, 5);                        \
	ddst7 = vis_faligndata(ddst1, ddst6);                   \
	ddst6 = vis_faligndata(ddst6, dsrc0);                   \
	(void *)vis_alignaddr(dpsrc, 4);                        \
	ddst1 = vis_faligndata(ddst5, ddst7);                   \
	(void *)vis_alignaddr(dpsrc, 1);                        \
	ddst2 = vis_faligndata(dsrc0, dsrc1);                   \
	(void *)vis_alignaddr(dpsrc, 6);                        \
	ddst2 = vis_faligndata(ddst2, ddst6)

/* *********************************************************** */

void
mlib_v_ImageFlipY_U8_3(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_d64 dsrc0, dsrc1, dsrc2;
	mlib_d64 dsrc00, dsrc11, dsrc22, dsrc33;
	mlib_d64 ddst0, ddst1, ddst2, ddst3;
	mlib_d64 ddst4, ddst5, ddst6, ddst7;
	mlib_s32 even_8;
	mlib_d64 *dpsrc;
	mlib_s32 i, j;
	mlib_s32 d_tail;
	mlib_u8 *psrc, *pdst;

	PREPAREVARS(mlib_u8, 3);

	if (sw < 8) {
		STRIP_3(ddata, sdata, sw, sh, mlib_u8);
		return;
	}

	for (i = 0; i < sh; i++) {
		psrc = sdata + i * sstride;
		pdst = ddata + i * dstride + 3 * sw;

		for (j = 0; ((mlib_addr)pdst & 7) != 0; j++) {
			pdst[-3] = psrc[3 * j];
			pdst[-2] = psrc[3 * j + 1];
			pdst[-1] = psrc[3 * j + 2];
			pdst -= 3;
		}

		psrc += 3 * j;

		d_tail = (sw - j) & 7;
		even_8 = (sw - j) >> 3;

		if (((mlib_addr)psrc & 7) != 0) {
			dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
			dsrc00 = dpsrc[0];
			dsrc11 = vis_ld_d64_nf(dpsrc + 1);
			dsrc22 = vis_ld_d64_nf(dpsrc + 2);
			dsrc33 = vis_ld_d64_nf(dpsrc + 3);
			dsrc0 = vis_faligndata(dsrc00, dsrc11);
			dsrc1 = vis_faligndata(dsrc11, dsrc22);
			dsrc2 = vis_faligndata(dsrc22, dsrc33);

#pragma pipeloop(0)
			for (j = 0; j < even_8; j++) {
				dsrc00 = dsrc33;
				dsrc11 = vis_ld_d64_nf(dpsrc + 4);
				dsrc22 = vis_ld_d64_nf(dpsrc + 5);
				dsrc33 = vis_ld_d64_nf(dpsrc + 6);
				CHANGE_U8_3;
				(void *)vis_alignaddr(psrc, 0);
				pdst -= 24;
				((mlib_d64 *)pdst)[0] = ddst0;
				dsrc0 = vis_faligndata(dsrc00, dsrc11);
				((mlib_d64 *)pdst)[1] = ddst1;
				dsrc1 = vis_faligndata(dsrc11, dsrc22);
				((mlib_d64 *)pdst)[2] = ddst2;
				dsrc2 = vis_faligndata(dsrc22, dsrc33);
				dpsrc += 3;
			}
		} else {
			dpsrc = (mlib_d64 *)psrc;
			dsrc0 = dpsrc[0];
			dsrc1 = vis_ld_d64_nf(dpsrc + 1);
			dsrc2 = vis_ld_d64_nf(dpsrc + 2);

#pragma pipeloop(0)
			for (j = 0; j < even_8; j++) {
				dsrc00 = vis_ld_d64_nf(dpsrc + 3);
				dsrc11 = vis_ld_d64_nf(dpsrc + 4);
				dsrc22 = vis_ld_d64_nf(dpsrc + 5);
				CHANGE_U8_3;
				pdst -= 24;
				((mlib_d64 *)pdst)[0] = ddst0;
				dsrc0 = dsrc00;
				((mlib_d64 *)pdst)[1] = ddst1;
				dsrc1 = dsrc11;
				((mlib_d64 *)pdst)[2] = ddst2;
				dsrc2 = dsrc22;
				dpsrc += 3;
			}
		}

		psrc += even_8 * 24;

		for (j = 0; j < d_tail; j++) {
			pdst[3 * (-j - 1)] = psrc[3 * j];
			pdst[3 * (-j - 1) + 1] = psrc[3 * j + 1];
			pdst[3 * (-j - 1) + 2] = psrc[3 * j + 2];
		}
	}
}

/* *********************************************************** */

#define	CHANGE_S16_3                                            \
	(void *)vis_alignaddr(dpsrc, 4);                        \
	ddst3 = vis_faligndata(dsrc1, dsrc1);                   \
	(void *)vis_alignaddr(dpsrc, 2);                        \
	ddst0 = vis_faligndata(dsrc2, ddst3);                   \
	ddst1 = vis_faligndata(dsrc1, dsrc2);                   \
	(void *)vis_alignaddr(dpsrc, 6);                        \
	ddst2 = vis_faligndata(ddst3, dsrc0);                   \
	ddst3 = vis_faligndata(dsrc0, dsrc1);                   \
	(void *)vis_alignaddr(dpsrc, 4);                        \
	ddst1 = vis_faligndata(ddst1, ddst3)

/* *********************************************************** */

void
mlib_v_ImageFlipY_S16_3(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_d64 dsrc0, dsrc1, dsrc2;
	mlib_d64 dsrc00, dsrc11, dsrc22, dsrc33;
	mlib_d64 ddst0, ddst1, ddst2, ddst3;
	mlib_s32 even_4;
	mlib_d64 *dpsrc;
	mlib_s32 i, j;
	mlib_s32 d_head, d_tail;
	mlib_u16 *psrc, *pdst;

	PREPAREVARS(mlib_u16, 3);

	if (sw < 4) {
		STRIP_3(ddata, sdata, sw, sh, mlib_u16);
		return;
	}

	for (i = 0; i < sh; i++) {
		psrc = sdata + i * sstride;
		pdst = ddata + i * dstride + 3 * sw;
		d_head = ((8 - (mlib_addr)pdst) & 7) >> 1;

		for (j = 0; j < d_head; j++) {
			pdst[-3] = psrc[3 * j];
			pdst[-2] = psrc[3 * j + 1];
			pdst[-1] = psrc[3 * j + 2];
			pdst -= 3;
		}

		psrc += 3 * d_head;

		d_tail = (sw - d_head) & 3;
		even_4 = (sw - d_head) >> 2;

		if (((mlib_addr)psrc & 7) != 0) {
			dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
			dsrc00 = dpsrc[0];
			dsrc11 = vis_ld_d64_nf(dpsrc + 1);
			dsrc22 = vis_ld_d64_nf(dpsrc + 2);
			dsrc33 = vis_ld_d64_nf(dpsrc + 3);
			dsrc0 = vis_faligndata(dsrc00, dsrc11);
			dsrc1 = vis_faligndata(dsrc11, dsrc22);
			dsrc2 = vis_faligndata(dsrc22, dsrc33);

#pragma pipeloop(0)
			for (j = 0; j < even_4; j++) {
				dsrc00 = dsrc33;
				dsrc11 = vis_ld_d64_nf(dpsrc + 4);
				dsrc22 = vis_ld_d64_nf(dpsrc + 5);
				dsrc33 = vis_ld_d64_nf(dpsrc + 6);
				CHANGE_S16_3;
				(void *)vis_alignaddr(psrc, 0);
				pdst -= 12;
				((mlib_d64 *)pdst)[0] = ddst0;
				dsrc0 = vis_faligndata(dsrc00, dsrc11);
				((mlib_d64 *)pdst)[1] = ddst1;
				dsrc1 = vis_faligndata(dsrc11, dsrc22);
				((mlib_d64 *)pdst)[2] = ddst2;
				dsrc2 = vis_faligndata(dsrc22, dsrc33);
				dpsrc += 3;
			}
		} else {
			dpsrc = (mlib_d64 *)psrc;
			dsrc0 = dpsrc[0];
			dsrc1 = vis_ld_d64_nf(dpsrc + 1);
			dsrc2 = vis_ld_d64_nf(dpsrc + 2);

#pragma pipeloop(0)
			for (j = 0; j < even_4; j++) {
				dsrc00 = vis_ld_d64_nf(dpsrc + 3);
				dsrc11 = vis_ld_d64_nf(dpsrc + 4);
				dsrc22 = vis_ld_d64_nf(dpsrc + 5);
				CHANGE_S16_3;
				pdst -= 12;
				((mlib_d64 *)pdst)[0] = ddst0;
				dsrc0 = dsrc00;
				((mlib_d64 *)pdst)[1] = ddst1;
				dsrc1 = dsrc11;
				((mlib_d64 *)pdst)[2] = ddst2;
				dsrc2 = dsrc22;
				dpsrc += 3;
			}
		}

		psrc += even_4 * 12;

		for (j = 0; j < d_tail; j++) {
			pdst[3 * (-j - 1)] = psrc[3 * j];
			pdst[3 * (-j - 1) + 1] = psrc[3 * j + 1];
			pdst[3 * (-j - 1) + 2] = psrc[3 * j + 2];
		}
	}
}

/* *********************************************************** */

#define	CHANGE_S32_3                                            \
	(void *)vis_alignaddr(dpsrc, 4);                        \
	ddst0 = vis_faligndata(dsrc1, dsrc2);                   \
	ddst1 = vis_faligndata(dsrc2, dsrc0);                   \
	ddst2 = vis_faligndata(dsrc0, dsrc1)

/* *********************************************************** */

void
mlib_v_ImageFlipY_S32_3(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_d64 dsrc0, dsrc1, dsrc2;
	mlib_d64 dsrc00, dsrc11, dsrc22, dsrc33;
	mlib_d64 ddst0, ddst1, ddst2;
	mlib_s32 even_2;
	mlib_d64 *dpsrc;
	mlib_s32 i, j;
	mlib_s32 d_head, d_tail;
	mlib_u32 *psrc, *pdst;

	PREPAREVARS(mlib_u32, 3);

	if (sw < 2) {
		STRIP_3(ddata, sdata, sw, sh, mlib_u32);
		return;
	}

	for (i = 0; i < sh; i++) {
		psrc = sdata + i * sstride;
		pdst = ddata + i * dstride + 3 * sw;

		if (d_head = ((8 - (mlib_addr)pdst) & 7) >> 2) {
			pdst[-3] = psrc[0];
			pdst[-2] = psrc[1];
			pdst[-1] = psrc[2];
			pdst -= 3;
		}

		psrc += 3 * d_head;

		d_tail = (sw - d_head) & 1;
		even_2 = (sw - d_head) >> 1;

		if (((mlib_addr)psrc & 7) != 0) {
			dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
			dsrc00 = dpsrc[0];
			dsrc11 = dpsrc[1];
			dsrc22 = vis_ld_d64_nf(dpsrc + 2);
			dsrc33 = vis_ld_d64_nf(dpsrc + 3);
			dsrc0 = vis_faligndata(dsrc00, dsrc11);
			dsrc1 = vis_faligndata(dsrc11, dsrc22);
			dsrc2 = vis_faligndata(dsrc22, dsrc33);

#pragma pipeloop(0)
			for (j = 0; j < even_2; j++) {
				dsrc00 = dsrc33;
				dsrc11 = vis_ld_d64_nf(dpsrc + 4);
				dsrc22 = vis_ld_d64_nf(dpsrc + 5);
				dsrc33 = vis_ld_d64_nf(dpsrc + 6);
				CHANGE_S32_3;
				(void *)vis_alignaddr(psrc, 0);
				pdst -= 6;
				((mlib_d64 *)pdst)[0] = ddst0;
				dsrc0 = vis_faligndata(dsrc00, dsrc11);
				((mlib_d64 *)pdst)[1] = ddst1;
				dsrc1 = vis_faligndata(dsrc11, dsrc22);
				((mlib_d64 *)pdst)[2] = ddst2;
				dsrc2 = vis_faligndata(dsrc22, dsrc33);
				dpsrc += 3;
			}
		} else {
			dpsrc = (mlib_d64 *)psrc;
			dsrc0 = dpsrc[0];
			dsrc1 = dpsrc[1];
			dsrc2 = vis_ld_d64_nf(dpsrc + 2);

#pragma pipeloop(0)
			for (j = 0; j < even_2; j++) {
				dsrc00 = vis_ld_d64_nf(dpsrc + 3);
				dsrc11 = vis_ld_d64_nf(dpsrc + 4);
				dsrc22 = vis_ld_d64_nf(dpsrc + 5);
				CHANGE_S32_3;
				pdst -= 6;
				((mlib_d64 *)pdst)[0] = ddst0;
				dsrc0 = dsrc00;
				((mlib_d64 *)pdst)[1] = ddst1;
				dsrc1 = dsrc11;
				((mlib_d64 *)pdst)[2] = ddst2;
				dsrc2 = dsrc22;
				dpsrc += 3;
			}
		}

		psrc += even_2 * 6;

		for (j = 0; j < d_tail; j++) {
			pdst[3 * (-j - 1)] = psrc[3 * j];
			pdst[3 * (-j - 1) + 1] = psrc[3 * j + 1];
			pdst[3 * (-j - 1) + 2] = psrc[3 * j + 2];
		}
	}
}

/* *********************************************************** */

#define	STRIP_4(pd, ps, w, h, data_type)                            \
	{                                                           \
	    mlib_s32 i, j, _w = (w), _h = (h);                      \
	    data_type *_ps = (ps), *_pd = (pd);                     \
	    data_type s0, s1, s2, s3;                               \
	                                                            \
	    for (i = 0; i < _h; i++) {                              \
		for (j = 0; j < _w; j++) {                          \
		    s0 = _ps[i * sstride + 4 * j];                  \
		    s1 = _ps[i * sstride + 4 * j + 1];              \
		    s2 = _ps[i * sstride + 4 * j + 2];              \
		    s3 = _ps[i * sstride + 4 * j + 3];              \
		    _pd[i * dstride + 4 * (_w - j - 1)] = s0;       \
		    _pd[i * dstride + 4 * (_w - j - 1) + 1] = s1;   \
		    _pd[i * dstride + 4 * (_w - j - 1) + 2] = s2;   \
		    _pd[i * dstride + 4 * (_w - j - 1) + 3] = s3;   \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

void
mlib_v_ImageFlipY_S16_4(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 even_2;
	mlib_d64 *dpsrc, *dpdst;
	mlib_d64 dsrc0, dsrc1, ddst;
	mlib_s32 i, j;
	mlib_u16 *psrc, *pdst;
	mlib_s32 emask_hi, emask_lo;
	mlib_s32 all_dst;

	PREPAREVARS(mlib_u16, 4);

	for (i = 0; i < sh; i++) {
		psrc = sdata + i * sstride;
		pdst = ddata + i * dstride + 4 * sw;

		if (!(((mlib_addr)pdst | (mlib_addr)psrc) & 7)) {
			if (sw & 1) {
				*((mlib_d64 *)(pdst - 4 * sw)) =
				    *((mlib_d64 *)(psrc + (sw - 1) * 4));
			}
#pragma pipeloop(0)
			for (j = 0; j <= (sw - 2); j += 2) {
				mlib_d64 src0 = *((mlib_d64 *)(psrc + j * 4));
				mlib_d64 src1 =
				    *((mlib_d64 *)(psrc + j * 4 + 4));

				*((mlib_d64 *)(pdst - j * 4 - 4)) = src0;
				*((mlib_d64 *)(pdst - j * 4 - 8)) = src1;
			}
		} else {
			even_2 = sw;

			dpdst = (mlib_d64 *)vis_alignaddr(pdst, 8);
			emask_hi = vis_edge8(pdst, dpdst);
			emask_lo = ~emask_hi;
			all_dst = ~(mlib_addr)pdst;

			dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
			dsrc0 = dpsrc[0];
			dsrc1 = dpsrc[1];
			ddst = vis_faligndata(dsrc0, dsrc1);

#pragma pipeloop(0)
			for (j = 0; j < even_2; j++) {
				dsrc0 = dsrc1;
				dsrc1 = dpsrc[2];
				dpdst--;
				NASTORE_U8;
				(void *)vis_alignaddr(psrc, 0);
				ddst = vis_faligndata(dsrc0, dsrc1);
				dpsrc++;
			}
		}
	}
}

/* *********************************************************** */
