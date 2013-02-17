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

#pragma ident	"@(#)mlib_v_ImageExtrema2.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageExtrema2_fast  - fast version of mlib_ImageExtrema2
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageStat.h>

/* *********************************************************** */

static void mlib_v_ImageExtrema2_fast_U8_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageExtrema2_fast_S16_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageExtrema2_fast_U16_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageExtrema2_fast_8_16(
    mlib_d64 *dres,
    mlib_s32 *size,
    const mlib_image *img);

static void mlib_v_ImageExtrema2_fast_S32_124_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageExtrema2_fast_S32_3_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageExtrema2_fast_S32(
    mlib_d64 *dres,
    mlib_s32 *size,
    const mlib_image *img);

/* *********************************************************** */

#define	PXL_MIN(res, x)	res = (x < res) ? x : res
#define	PXL_MAX(res, x)	res = (x > res) ? x : res

/* *********************************************************** */

#define	MAIN_LOOP                                               \
	for (i = 0; i <= n - 24; i += 24) {                     \
	    mlib_d64 sd0, sd1, sd2, sd3, sd4, sd5;              \
	    mlib_d64 lmin0, lmin1, lmin2;                       \
	    mlib_d64 lmax0, lmax1, lmax2;                       \
	    mlib_s32 m_0, m_1, m_2, m_3, m_4, m_5;              \
	                                                        \
	    LOAD_1(sd0, sd1);                                   \
	    LOAD_2(sd2, sd3);                                   \
	    LOAD_1(sd4, sd5);                                   \
	    m_0 = vis_fcmpgt16(sd0, sd3);                       \
	    m_1 = vis_fcmpgt16(sd1, sd4);                       \
	    m_2 = vis_fcmpgt16(sd2, sd5);                       \
	    vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);     \
	    lmin0 = vis_bshuffle(sd3, sd0);                     \
	    lmax0 = vis_bshuffle(sd0, sd3);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);     \
	    lmin1 = vis_bshuffle(sd4, sd1);                     \
	    lmax1 = vis_bshuffle(sd1, sd4);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_2], 0);     \
	    lmin2 = vis_bshuffle(sd5, sd2);                     \
	    lmax2 = vis_bshuffle(sd2, sd5);                     \
	    m_0 = vis_fcmpgt16(dd0, lmin0);                     \
	    m_1 = vis_fcmpgt16(dd1, lmin1);                     \
	    m_2 = vis_fcmpgt16(dd2, lmin2);                     \
	    m_3 = vis_fcmplt16(dd3, lmax0);                     \
	    m_4 = vis_fcmplt16(dd4, lmax1);                     \
	    m_5 = vis_fcmplt16(dd5, lmax2);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);     \
	    dd0 = vis_bshuffle(lmin0, dd0);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);     \
	    dd1 = vis_bshuffle(lmin1, dd1);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_2], 0);     \
	    dd2 = vis_bshuffle(lmin2, dd2);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_3], 0);     \
	    dd3 = vis_bshuffle(lmax0, dd3);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_4], 0);     \
	    dd4 = vis_bshuffle(lmax1, dd4);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_5], 0);     \
	    dd5 = vis_bshuffle(lmax2, dd5);                     \
	    MOVE_SRC;                                           \
	}

/* *********************************************************** */

#define	TAIL                                                          \
	if (i < n) {                                                  \
	    mlib_d64 sd0, sd1;                                        \
	    mlib_s32 m_0, m_1, m_2, m_3;                              \
	                                                              \
	    if ((n - i) < 8) {                                        \
		emask = 0xFF << (8 - (n - i));                        \
	    } else {                                                  \
		emask = 0xFF;                                         \
	    }                                                         \
	    LOAD_1(sd0, sd1);                                         \
	    m_0 = vis_fcmpgt16(dd0, sd0);                             \
	    m_1 = vis_fcmpgt16(dd1, sd1);                             \
	    m_2 = vis_fcmplt16(dd3, sd0);                             \
	    m_3 = vis_fcmplt16(dd4, sd1);                             \
	    m_0 &= ((emask >> 4) & 0xF);                              \
	    m_1 &= (emask & 0xF);                                     \
	    m_2 &= ((emask >> 4) & 0xF);                              \
	    m_3 &= (emask & 0xF);                                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);           \
	    dd0 = vis_bshuffle(sd0, dd0);                             \
	    vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);           \
	    dd1 = vis_bshuffle(sd1, dd1);                             \
	    vis_write_bmask(mlib_extrema_mask_s16[m_2], 0);           \
	    dd3 = vis_bshuffle(sd0, dd3);                             \
	    vis_write_bmask(mlib_extrema_mask_s16[m_3], 0);           \
	    dd4 = vis_bshuffle(sd1, dd4);                             \
	    i += 8;                                                   \
	    if (i < n) {                                              \
		if ((n - i) < 8) {                                    \
		    emask = 0xFF << (8 - (n - i));                    \
		} else {                                              \
		    emask = 0xFF;                                     \
		}                                                     \
		LOAD_2(sd0, sd1);                                     \
		m_0 = vis_fcmpgt16(dd2, sd0);                         \
		m_1 = vis_fcmpgt16(dd0, sd1);                         \
		m_2 = vis_fcmplt16(dd5, sd0);                         \
		m_3 = vis_fcmplt16(dd3, sd1);                         \
		m_0 &= ((emask >> 4) & 0xF);                          \
		m_1 &= (emask & 0xF);                                 \
		m_2 &= ((emask >> 4) & 0xF);                          \
		m_3 &= (emask & 0xF);                                 \
		vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);       \
		dd2 = vis_bshuffle(sd0, dd2);                         \
		vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);       \
		dd0 = vis_bshuffle(sd1, dd0);                         \
		vis_write_bmask(mlib_extrema_mask_s16[m_2], 0);       \
		dd5 = vis_bshuffle(sd0, dd5);                         \
		vis_write_bmask(mlib_extrema_mask_s16[m_3], 0);       \
		dd3 = vis_bshuffle(sd1, dd3);                         \
		i += 8;                                               \
		if (i < n) {                                          \
		    emask = 0xFF << (8 - (n - i));                    \
		    LOAD_1(sd0, sd1);                                 \
		    m_0 = vis_fcmpgt16(dd1, sd0);                     \
		    m_1 = vis_fcmpgt16(dd2, sd1);                     \
		    m_2 = vis_fcmplt16(dd4, sd0);                     \
		    m_3 = vis_fcmplt16(dd5, sd1);                     \
		    m_0 &= ((emask >> 4) & 0xF);                      \
		    m_1 &= (emask & 0xF);                             \
		    m_2 &= ((emask >> 4) & 0xF);                      \
		    m_3 &= (emask & 0xF);                             \
		    vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);   \
		    dd1 = vis_bshuffle(sd0, dd1);                     \
		    vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);   \
		    dd2 = vis_bshuffle(sd1, dd2);                     \
		    vis_write_bmask(mlib_extrema_mask_s16[m_2], 0);   \
		    dd4 = vis_bshuffle(sd0, dd4);                     \
		    vis_write_bmask(mlib_extrema_mask_s16[m_3], 0);   \
		    dd5 = vis_bshuffle(sd1, dd5);                     \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

mlib_status
mlib_ImageExtrema2_fast(
    mlib_s32 *min,
    mlib_s32 *max,
    const mlib_image *img)
{
	mlib_type type;
	mlib_d64 dres[6];
	mlib_s32 *ires = (mlib_s32 *)dres;
	mlib_s16 *sres = (mlib_s16 *)dres;
	mlib_u16 *ures = (mlib_u16 *)dres;
	mlib_s32 nchan, size[1], num;
	mlib_s32 i, k;

	MLIB_IMAGE_CHECK(img);

	if (min == NULL)
		return (MLIB_NULLPOINTER);

	if (max == NULL)
		return (MLIB_NULLPOINTER);

	type = mlib_ImageGetType(img);

	if ((type != MLIB_BYTE) && (type != MLIB_SHORT) &&
	    (type != MLIB_USHORT) && (type != MLIB_INT))
		return (MLIB_FAILURE);

	nchan = mlib_ImageGetChannels(img);

	if (nchan < 0 || nchan > 4)
		return (MLIB_FAILURE);

	if (type == MLIB_INT) {

		mlib_v_ImageExtrema2_fast_S32(dres, size, img);

		if (nchan == 3) {
			num = (size[0] < 6) ? size[0] : 6;
		} else {
			num = (size[0] < 4) ? size[0] : 4;
		}

		for (k = 0; k < nchan; k++) {
			min[k] = ires[k];
			for (i = k + nchan; i < num; i += nchan)
				PXL_MIN(min[k], ires[i]);
		}

		for (k = 0; k < nchan; k++) {
			max[k] = ires[k + 6];
			for (i = k + nchan; i < num; i += nchan)
				PXL_MAX(max[k], ires[i + 6]);
		}

	} else {

		mlib_v_ImageExtrema2_fast_8_16(dres, size, img);

		num = (size[0] < 12) ? size[0] : 12;

		if (type == MLIB_USHORT) {
			for (k = 0; k < nchan; k++) {
				min[k] = ures[k];
				for (i = k + nchan; i < num; i += nchan)
					PXL_MIN(min[k], ures[i]);
			}

			for (k = 0; k < nchan; k++) {
				max[k] = ures[k + 12];
				for (i = k + nchan; i < num; i += nchan)
					PXL_MAX(max[k], ures[i + 12]);
			}

		} else {
			for (k = 0; k < nchan; k++) {
				min[k] = sres[k];
				for (i = k + nchan; i < num; i += nchan)
					PXL_MIN(min[k], sres[i]);
			}

			for (k = 0; k < nchan; k++) {
				max[k] = sres[k + 12];
				for (i = k + nchan; i < num; i += nchan)
					PXL_MAX(max[k], sres[i + 12]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef LOAD_1
#undef LOAD_2
#undef MOVE_SRC

/* *********************************************************** */

#define	LOAD_1(sd0, sd1)                                        \
	s1 = (*sp++);                                           \
	s01 = vis_faligndata(s0, s1);                           \
	sd0 = vis_fmul8x16al(vis_read_hi(s01), fone);           \
	sd1 = vis_fmul8x16al(vis_read_lo(s01), fone)

/* *********************************************************** */

#define	LOAD_2(sd0, sd1)                                        \
	s0 = (*sp++);                                           \
	s01 = vis_faligndata(s1, s0);                           \
	sd0 = vis_fmul8x16al(vis_read_hi(s01), fone);           \
	sd1 = vis_fmul8x16al(vis_read_lo(s01), fone)

#define	MOVE_SRC	s0 = s1

/* *********************************************************** */

void
mlib_v_ImageExtrema2_fast_U8_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_d64 dd0, dd1, dd2, dd3, dd4, dd5;
	mlib_s32 emask;
	mlib_d64 s01, s0, s1;
	mlib_s32 i;

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];
	dd2 = pres[2];
	dd3 = pres[3];
	dd4 = pres[4];
	dd5 = pres[5];

	s0 = (*sp++);

#pragma pipeloop(0)
	MAIN_LOOP;

	TAIL;

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
	pres[3] = dd3;
	pres[4] = dd4;
	pres[5] = dd5;
}

/* *********************************************************** */

#undef LOAD_1
#undef LOAD_2
#undef MOVE_SRC

/* *********************************************************** */

#define	LOAD_1(sd0, sd1)                                        \
	s1 = (*sp++);                                           \
	sd0 = vis_faligndata(s0, s1);                           \
	s0 = (*sp++);                                           \
	sd1 = vis_faligndata(s1, s0)

/* *********************************************************** */

#define	LOAD_2	LOAD_1
#define	MOVE_SRC

/* *********************************************************** */

void
mlib_v_ImageExtrema2_fast_S16_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_d64 dd0, dd1, dd2, dd3, dd4, dd5;
	mlib_s32 emask;
	mlib_d64 s0, s1;
	mlib_s32 i;

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];
	dd2 = pres[2];
	dd3 = pres[3];
	dd4 = pres[4];
	dd5 = pres[5];

	s0 = (*sp++);

#pragma pipeloop(0)
	MAIN_LOOP;

	TAIL;

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
	pres[3] = dd3;
	pres[4] = dd4;
	pres[5] = dd5;
}

/* *********************************************************** */

#undef LOAD_1
#undef LOAD_2
#undef MOVE_SRC

/* *********************************************************** */

#define	LOAD_1(sd0, sd1)                                        \
	s1 = (*sp++);                                           \
	sd0 = vis_faligndata(s0, s1);                           \
	s0 = (*sp++);                                           \
	sd1 = vis_faligndata(s1, s0);                           \
	sd0 = vis_fxor(sd0, xor_mask);                          \
	sd1 = vis_fxor(sd1, xor_mask)

/* *********************************************************** */

#define	LOAD_2	LOAD_1
#define	MOVE_SRC

/* *********************************************************** */

void
mlib_v_ImageExtrema2_fast_U16_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_d64 dd0, dd1, dd2, dd3, dd4, dd5;
	mlib_s32 emask;
	mlib_d64 s0, s1;
	mlib_s32 i;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];
	dd2 = pres[2];
	dd3 = pres[3];
	dd4 = pres[4];
	dd5 = pres[5];

	s0 = (*sp++);

#pragma pipeloop(0)
	MAIN_LOOP;

	TAIL;

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
	pres[3] = dd3;
	pres[4] = dd4;
	pres[5] = dd5;
}

/* *********************************************************** */

void
mlib_v_ImageExtrema2_fast_8_16(
    mlib_d64 *dres,
    mlib_s32 *size,
    const mlib_image *img)
{
	mlib_type type;
	mlib_u8 *sl;
	mlib_s32 nchan, xsize, ysize, slb;
	mlib_s32 j;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

	MLIB_IMAGE_GET_ALL_PARAMS(img, type, nchan, xsize, ysize, slb, sl);
	xsize *= nchan;

	dres[0] = dres[1] = dres[2] = vis_to_double_dup(0x7fff7fff);
	dres[3] = dres[4] = dres[5] = vis_to_double_dup(0x80008000);

	if (type == MLIB_BYTE) {
		if (slb == xsize) {
			xsize *= ysize;
			ysize = 1;
		}

		for (j = 0; j < ysize; j++) {

			mlib_v_ImageExtrema2_fast_U8_D1(dres, sl, xsize);

			sl += slb;
		}

	} else if (type == MLIB_SHORT) {

		if (slb == xsize * 2) {
			xsize *= ysize;
			ysize = 1;
		}

		for (j = 0; j < ysize; j++) {

			mlib_v_ImageExtrema2_fast_S16_D1(dres, sl, xsize);

			sl += slb;
		}

	} else {

		if (slb == xsize * 2) {
			xsize *= ysize;
			ysize = 1;
		}

		for (j = 0; j < ysize; j++) {

			mlib_v_ImageExtrema2_fast_U16_D1(dres, sl, xsize);

			sl += slb;
		}

		dres[0] = vis_fxor(dres[0], xor_mask);
		dres[1] = vis_fxor(dres[1], xor_mask);
		dres[2] = vis_fxor(dres[2], xor_mask);
		dres[3] = vis_fxor(dres[3], xor_mask);
		dres[4] = vis_fxor(dres[4], xor_mask);
		dres[5] = vis_fxor(dres[5], xor_mask);
	}

	*size = xsize;
}

/* *********************************************************** */

void
mlib_v_ImageExtrema2_fast_S32_124_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_d64 dd0, dd1, dd3, dd4;
	mlib_s32 emask;
	mlib_d64 s0, s1;
	mlib_s32 i;

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];
	dd3 = pres[3];
	dd4 = pres[4];

	s0 = (*sp++);

#pragma pipeloop(0)
	for (i = 0; i <= n - 8; i += 8) {
		mlib_d64 sd0, sd1, sd2, sd3;
		mlib_d64 lmin0, lmin1;
		mlib_d64 lmax0, lmax1;
		mlib_s32 m_0, m_1, m_2, m_3;

		s1 = (*sp++);
		sd0 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd1 = vis_faligndata(s1, s0);
		s1 = (*sp++);
		sd2 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd3 = vis_faligndata(s1, s0);

		m_0 = vis_fcmpgt32(sd0, sd2);
		m_1 = vis_fcmpgt32(sd1, sd3);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		lmin0 = vis_bshuffle(sd2, sd0);
		lmax0 = vis_bshuffle(sd0, sd2);

		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		lmin1 = vis_bshuffle(sd3, sd1);
		lmax1 = vis_bshuffle(sd1, sd3);

		m_0 = vis_fcmpgt32(dd0, lmin0);
		m_1 = vis_fcmpgt32(dd1, lmin1);

		m_2 = vis_fcmplt32(dd3, lmax0);
		m_3 = vis_fcmplt32(dd4, lmax1);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		dd0 = vis_bshuffle(lmin0, dd0);

		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		dd1 = vis_bshuffle(lmin1, dd1);

		vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
		dd3 = vis_bshuffle(lmax0, dd3);

		vis_write_bmask(mlib_extrema_mask_s32[m_3], 0);
		dd4 = vis_bshuffle(lmax1, dd4);
	}

	if (i < n) {
		mlib_d64 sd0, sd1;
		mlib_s32 m_0, m_1, m_2, m_3;

		if ((n - i) < 4) {
			emask = 0xFF << (4 - (n - i));
		} else {
			emask = 0xFF;
		}

		s1 = (*sp++);
		sd0 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd1 = vis_faligndata(s1, s0);

		m_0 = vis_fcmpgt32(dd0, sd0);
		m_1 = vis_fcmpgt32(dd1, sd1);
		m_2 = vis_fcmplt32(dd3, sd0);
		m_3 = vis_fcmplt32(dd4, sd1);

		m_0 &= ((emask >> 2) & 0x3);
		m_1 &= (emask & 0x3);
		m_2 &= ((emask >> 2) & 0x3);
		m_3 &= (emask & 0x3);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		dd0 = vis_bshuffle(sd0, dd0);
		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		dd1 = vis_bshuffle(sd1, dd1);

		vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
		dd3 = vis_bshuffle(sd0, dd3);
		vis_write_bmask(mlib_extrema_mask_s32[m_3], 0);
		dd4 = vis_bshuffle(sd1, dd4);

		i += 4;

		if (i < n) {

			emask = 0xFF << (4 - (n - i));

			s1 = (*sp++);
			sd0 = vis_faligndata(s0, s1);
			s0 = (*sp++);
			sd1 = vis_faligndata(s1, s0);

			m_0 = vis_fcmpgt32(dd0, sd0);
			m_1 = vis_fcmpgt32(dd1, sd1);
			m_2 = vis_fcmplt32(dd3, sd0);
			m_3 = vis_fcmplt32(dd4, sd1);

			m_0 &= ((emask >> 2) & 0x3);
			m_1 &= (emask & 0x3);
			m_2 &= ((emask >> 2) & 0x3);
			m_3 &= (emask & 0x3);

			vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
			dd0 = vis_bshuffle(sd0, dd0);
			vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
			dd1 = vis_bshuffle(sd1, dd1);

			vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
			dd3 = vis_bshuffle(sd0, dd3);
			vis_write_bmask(mlib_extrema_mask_s32[m_3], 0);
			dd4 = vis_bshuffle(sd1, dd4);
		}
	}

	pres[0] = dd0;
	pres[1] = dd1;
	pres[3] = dd3;
	pres[4] = dd4;
}

/* *********************************************************** */

void
mlib_v_ImageExtrema2_fast_S32_3_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_d64 dd0, dd1, dd2, dd3, dd4, dd5;
	mlib_s32 emask;
	mlib_d64 s0, s1;
	mlib_s32 i;

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];
	dd2 = pres[2];
	dd3 = pres[3];
	dd4 = pres[4];
	dd5 = pres[5];

	s0 = (*sp++);

#pragma pipeloop(0)
	for (i = 0; i <= n - 12; i += 12) {
		mlib_d64 sd0, sd1, sd2, sd3, sd4, sd5;
		mlib_d64 lmin0, lmin1, lmin2;
		mlib_d64 lmax0, lmax1, lmax2;
		mlib_s32 m_0, m_1, m_2, m_3, m_4, m_5;

		s1 = (*sp++);
		sd0 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd1 = vis_faligndata(s1, s0);
		s1 = (*sp++);
		sd2 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd3 = vis_faligndata(s1, s0);
		s1 = (*sp++);
		sd4 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd5 = vis_faligndata(s1, s0);

		m_0 = vis_fcmpgt32(sd0, sd3);
		m_1 = vis_fcmpgt32(sd1, sd4);
		m_2 = vis_fcmpgt32(sd2, sd5);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		lmin0 = vis_bshuffle(sd3, sd0);
		lmax0 = vis_bshuffle(sd0, sd3);

		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		lmin1 = vis_bshuffle(sd4, sd1);
		lmax1 = vis_bshuffle(sd1, sd4);

		vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
		lmin2 = vis_bshuffle(sd5, sd2);
		lmax2 = vis_bshuffle(sd2, sd5);

		m_0 = vis_fcmpgt32(dd0, lmin0);
		m_1 = vis_fcmpgt32(dd1, lmin1);
		m_2 = vis_fcmpgt32(dd2, lmin2);

		m_3 = vis_fcmplt32(dd3, lmax0);
		m_4 = vis_fcmplt32(dd4, lmax1);
		m_5 = vis_fcmplt32(dd5, lmax2);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		dd0 = vis_bshuffle(lmin0, dd0);

		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		dd1 = vis_bshuffle(lmin1, dd1);

		vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
		dd2 = vis_bshuffle(lmin2, dd2);

		vis_write_bmask(mlib_extrema_mask_s32[m_3], 0);
		dd3 = vis_bshuffle(lmax0, dd3);

		vis_write_bmask(mlib_extrema_mask_s32[m_4], 0);
		dd4 = vis_bshuffle(lmax1, dd4);

		vis_write_bmask(mlib_extrema_mask_s32[m_5], 0);
		dd5 = vis_bshuffle(lmax2, dd5);
	}

	if (i < n) {
		mlib_d64 sd0, sd1;
		mlib_s32 m_0, m_1, m_2, m_3;

		if ((n - i) < 4) {
			emask = 0xFF << (4 - (n - i));
		} else {
			emask = 0xFF;
		}

		s1 = (*sp++);
		sd0 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd1 = vis_faligndata(s1, s0);

		m_0 = vis_fcmpgt32(dd0, sd0);
		m_1 = vis_fcmpgt32(dd1, sd1);
		m_2 = vis_fcmplt32(dd3, sd0);
		m_3 = vis_fcmplt32(dd4, sd1);

		m_0 &= ((emask >> 2) & 0x3);
		m_1 &= (emask & 0x3);
		m_2 &= ((emask >> 2) & 0x3);
		m_3 &= (emask & 0x3);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		dd0 = vis_bshuffle(sd0, dd0);
		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		dd1 = vis_bshuffle(sd1, dd1);

		vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
		dd3 = vis_bshuffle(sd0, dd3);
		vis_write_bmask(mlib_extrema_mask_s32[m_3], 0);
		dd4 = vis_bshuffle(sd1, dd4);

		i += 4;

		if (i < n) {
			if ((n - i) < 4) {
				emask = 0xFF << (4 - (n - i));
			} else {
				emask = 0xFF;
			}

			s1 = (*sp++);
			sd0 = vis_faligndata(s0, s1);
			s0 = (*sp++);
			sd1 = vis_faligndata(s1, s0);

			m_0 = vis_fcmpgt32(dd2, sd0);
			m_1 = vis_fcmpgt32(dd0, sd1);
			m_2 = vis_fcmplt32(dd5, sd0);
			m_3 = vis_fcmplt32(dd3, sd1);

			m_0 &= ((emask >> 2) & 0x3);
			m_1 &= (emask & 0x3);
			m_2 &= ((emask >> 2) & 0x3);
			m_3 &= (emask & 0x3);

			vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
			dd2 = vis_bshuffle(sd0, dd2);
			vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
			dd0 = vis_bshuffle(sd1, dd0);

			vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
			dd5 = vis_bshuffle(sd0, dd5);
			vis_write_bmask(mlib_extrema_mask_s32[m_3], 0);
			dd3 = vis_bshuffle(sd1, dd3);

			i += 4;

			if (i < n) {
				emask = 0xFF << (4 - (n - i));

				s1 = (*sp++);
				sd0 = vis_faligndata(s0, s1);
				s0 = (*sp++);
				sd1 = vis_faligndata(s1, s0);

				m_0 = vis_fcmpgt32(dd1, sd0);
				m_1 = vis_fcmpgt32(dd2, sd1);
				m_2 = vis_fcmplt32(dd4, sd0);
				m_3 = vis_fcmplt32(dd5, sd1);

				m_0 &= ((emask >> 2) & 0x3);
				m_1 &= (emask & 0x3);
				m_2 &= ((emask >> 2) & 0x3);
				m_3 &= (emask & 0x3);

				vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
				dd1 = vis_bshuffle(sd0, dd1);
				vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
				dd2 = vis_bshuffle(sd1, dd2);

				vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
				dd4 = vis_bshuffle(sd0, dd4);
				vis_write_bmask(mlib_extrema_mask_s32[m_3], 0);
				dd5 = vis_bshuffle(sd1, dd5);
			}
		}
	}

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
	pres[3] = dd3;
	pres[4] = dd4;
	pres[5] = dd5;
}

/* *********************************************************** */

void
mlib_v_ImageExtrema2_fast_S32(
    mlib_d64 *dres,
    mlib_s32 *size,
    const mlib_image *img)
{
	mlib_u8 *sl;
	mlib_s32 nchan, xsize, ysize, slb;
	mlib_s32 j;

	nchan = mlib_ImageGetChannels(img);
	xsize = mlib_ImageGetWidth(img);
	ysize = mlib_ImageGetHeight(img);
	slb = mlib_ImageGetStride(img);
	sl = (void *)mlib_ImageGetData(img);

	xsize *= nchan;

	dres[0] = dres[1] = dres[2] = vis_to_double_dup(0x7fffffff);
	dres[3] = dres[4] = dres[5] = vis_to_double_dup(0x80000000);

	if (slb == 4 * xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	if (nchan == 3) {
		for (j = 0; j < ysize; j++) {

			mlib_v_ImageExtrema2_fast_S32_3_D1(dres, sl, xsize);

			sl += slb;
		}

	} else {
		for (j = 0; j < ysize; j++) {

			mlib_v_ImageExtrema2_fast_S32_124_D1(dres, sl, xsize);

			sl += slb;
		}
	}

	*size = xsize;
}

/* *********************************************************** */
