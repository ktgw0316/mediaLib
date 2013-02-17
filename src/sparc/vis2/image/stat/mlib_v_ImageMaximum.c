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

#pragma ident	"@(#)mlib_v_ImageMaximum.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageMaximum - calculate maximum for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMaximum(mlib_s32         *max,
 *                                    const mlib_image *img)
 *
 * ARGUMENTS
 *      max     pointer to max vector
 *      img     pointer to an image
 *
 * DESCRIPTION
 *
 *      max = MAX (img[i][j]);
 *            i, j
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageStat.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMaximum = __mlib_ImageMaximum

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMaximum) mlib_ImageMaximum
    __attribute__((weak, alias("__mlib_ImageMaximum")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_v_ImageMaximum_U8_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageMaximum_S16_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageMaximum_U16_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageMaximum_8_16(
    mlib_d64 *dres,
    mlib_s32 *size,
    const mlib_image *img);

static void mlib_v_ImageMaximum_S32_124_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageMaximum_S32_3_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n);

static void mlib_v_ImageMaximum_S32(
    mlib_d64 *dres,
    mlib_s32 *size,
    const mlib_image *img);

#define	PXL_MAX(res, x)	res = (x > res) ? x : res

/* *********************************************************** */

#define	MAIN_LOOP                                               \
	for (i = 0; i <= n - 24; i += 24) {                     \
	    mlib_d64 sd0, sd1, sd2, sd3, sd4, sd5;              \
	    mlib_d64 lmax0, lmax1, lmax2;                       \
	    mlib_s32 m_0, m_1, m_2;                             \
	                                                        \
	    LOAD_1(sd0, sd1);                                   \
	    LOAD_2(sd2, sd3);                                   \
	    LOAD_1(sd4, sd5);                                   \
	    m_0 = vis_fcmplt16(sd0, sd3);                       \
	    m_1 = vis_fcmplt16(sd1, sd4);                       \
	    m_2 = vis_fcmplt16(sd2, sd5);                       \
	    vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);     \
	    lmax0 = vis_bshuffle(sd3, sd0);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);     \
	    lmax1 = vis_bshuffle(sd4, sd1);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_2], 0);     \
	    lmax2 = vis_bshuffle(sd5, sd2);                     \
	    m_0 = vis_fcmplt16(dd0, lmax0);                     \
	    m_1 = vis_fcmplt16(dd1, lmax1);                     \
	    m_2 = vis_fcmplt16(dd2, lmax2);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);     \
	    dd0 = vis_bshuffle(lmax0, dd0);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);     \
	    dd1 = vis_bshuffle(lmax1, dd1);                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_2], 0);     \
	    dd2 = vis_bshuffle(lmax2, dd2);                     \
	    MOVE_SRC;                                           \
	}

/* *********************************************************** */

#define	TAIL                                                          \
	if (i < n) {                                                  \
	    mlib_d64 sd0, sd1;                                        \
	    mlib_s32 m_0, m_1;                                        \
	                                                              \
	    if ((n - i) < 8) {                                        \
		emask = 0xFF << (8 - (n - i));                        \
	    } else {                                                  \
		emask = 0xFF;                                         \
	    }                                                         \
	    LOAD_1(sd0, sd1);                                         \
	    m_0 = vis_fcmplt16(dd0, sd0);                             \
	    m_1 = vis_fcmplt16(dd1, sd1);                             \
	    m_0 &= ((emask >> 4) & 0xF);                              \
	    m_1 &= (emask & 0xF);                                     \
	    vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);           \
	    dd0 = vis_bshuffle(sd0, dd0);                             \
	    vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);           \
	    dd1 = vis_bshuffle(sd1, dd1);                             \
	    i += 8;                                                   \
	    if (i < n) {                                              \
		if ((n - i) < 8) {                                    \
		    emask = 0xFF << (8 - (n - i));                    \
		} else {                                              \
		    emask = 0xFF;                                     \
		}                                                     \
		LOAD_2(sd0, sd1);                                     \
		m_0 = vis_fcmplt16(dd2, sd0);                         \
		m_1 = vis_fcmplt16(dd0, sd1);                         \
		m_0 &= ((emask >> 4) & 0xF);                          \
		m_1 &= (emask & 0xF);                                 \
		vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);       \
		dd2 = vis_bshuffle(sd0, dd2);                         \
		vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);       \
		dd0 = vis_bshuffle(sd1, dd0);                         \
		i += 8;                                               \
		if (i < n) {                                          \
		    emask = 0xFF << (8 - (n - i));                    \
		    LOAD_1(sd0, sd1);                                 \
		    m_0 = vis_fcmplt16(dd1, sd0);                     \
		    m_1 = vis_fcmplt16(dd2, sd1);                     \
		    m_0 &= ((emask >> 4) & 0xF);                      \
		    m_1 &= (emask & 0xF);                             \
		    vis_write_bmask(mlib_extrema_mask_s16[m_0], 0);   \
		    dd1 = vis_bshuffle(sd0, dd1);                     \
		    vis_write_bmask(mlib_extrema_mask_s16[m_1], 0);   \
		    dd2 = vis_bshuffle(sd1, dd2);                     \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageMaximum(
    mlib_s32 *max,
    const mlib_image *img)
{
	mlib_type type;
	mlib_d64 dres[3];
	mlib_s32 *ires = (mlib_s32 *)dres;
	mlib_s16 *sres = (mlib_s16 *)dres;
	mlib_u16 *ures = (mlib_u16 *)dres;
	mlib_s32 nchan, size[1], num;
	mlib_s32 i, k;

	MLIB_IMAGE_CHECK(img);

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

		mlib_v_ImageMaximum_S32(dres, size, img);

		if (nchan == 3) {
			num = (size[0] < 6) ? size[0] : 6;
		} else {
			num = (size[0] < 4) ? size[0] : 4;
		}

		for (k = 0; k < nchan; k++) {
			max[k] = ires[k];
			for (i = k + nchan; i < num; i += nchan)
				PXL_MAX(max[k], ires[i]);
		}

	} else {

		mlib_v_ImageMaximum_8_16(dres, size, img);

		num = (size[0] < 12) ? size[0] : 12;

		if (type == MLIB_USHORT) {
			for (k = 0; k < nchan; k++) {
				max[k] = ures[k];
				for (i = k + nchan; i < num; i += nchan)
					PXL_MAX(max[k], ures[i]);
			}

		} else {
			for (k = 0; k < nchan; k++) {
				max[k] = sres[k];
				for (i = k + nchan; i < num; i += nchan)
					PXL_MAX(max[k], sres[i]);
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
mlib_v_ImageMaximum_U8_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 emask;
	mlib_d64 s01, s0, s1;
	mlib_s32 i;

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];
	dd2 = pres[2];

	s0 = (*sp++);

#pragma pipeloop(0)
	MAIN_LOOP;

	TAIL;

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
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
mlib_v_ImageMaximum_S16_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 emask;
	mlib_d64 s0, s1;
	mlib_s32 i;

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];
	dd2 = pres[2];

	s0 = (*sp++);

#pragma pipeloop(0)
	MAIN_LOOP;

	TAIL;

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
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
mlib_v_ImageMaximum_U16_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 emask;
	mlib_d64 s0, s1;
	mlib_s32 i;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];
	dd2 = pres[2];

	s0 = (*sp++);

#pragma pipeloop(0)
	MAIN_LOOP;

	TAIL;

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_8_16(
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

	dres[0] = dres[1] = dres[2] = vis_to_double_dup(0x80008000);

	if (type == MLIB_BYTE) {
		if (slb == xsize) {
			xsize *= ysize;
			ysize = 1;
		}

		for (j = 0; j < ysize; j++) {

			mlib_v_ImageMaximum_U8_D1(dres, sl, xsize);

			sl += slb;
		}

	} else if (type == MLIB_SHORT) {

		if (slb == xsize * 2) {
			xsize *= ysize;
			ysize = 1;
		}

		for (j = 0; j < ysize; j++) {

			mlib_v_ImageMaximum_S16_D1(dres, sl, xsize);

			sl += slb;
		}

	} else {

		if (slb == xsize * 2) {
			xsize *= ysize;
			ysize = 1;
		}

		for (j = 0; j < ysize; j++) {

			mlib_v_ImageMaximum_U16_D1(dres, sl, xsize);

			sl += slb;
		}

		dres[0] = vis_fxor(dres[0], xor_mask);
		dres[1] = vis_fxor(dres[1], xor_mask);
		dres[2] = vis_fxor(dres[2], xor_mask);
	}

	*size = xsize;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_S32_124_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_d64 dd0, dd1;
	mlib_s32 emask;
	mlib_d64 s0, s1;
	mlib_s32 i;

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];

	s0 = (*sp++);

#pragma pipeloop(0)
	for (i = 0; i <= n - 8; i += 8) {
		mlib_d64 sd0, sd1, sd2, sd3;
		mlib_d64 lmax0, lmax1;
		mlib_s32 m_0, m_1;

		s1 = (*sp++);
		sd0 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd1 = vis_faligndata(s1, s0);
		s1 = (*sp++);
		sd2 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd3 = vis_faligndata(s1, s0);

		m_0 = vis_fcmplt32(sd0, sd2);
		m_1 = vis_fcmplt32(sd1, sd3);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		lmax0 = vis_bshuffle(sd2, sd0);

		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		lmax1 = vis_bshuffle(sd3, sd1);

		m_0 = vis_fcmplt32(dd0, lmax0);
		m_1 = vis_fcmplt32(dd1, lmax1);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		dd0 = vis_bshuffle(lmax0, dd0);

		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		dd1 = vis_bshuffle(lmax1, dd1);
	}

	if (i < n) {
		mlib_d64 sd0, sd1;
		mlib_s32 m_0, m_1;

		if ((n - i) < 4) {
			emask = 0xFF << (4 - (n - i));
		} else {
			emask = 0xFF;
		}

		s1 = (*sp++);
		sd0 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd1 = vis_faligndata(s1, s0);

		m_0 = vis_fcmplt32(dd0, sd0);
		m_1 = vis_fcmplt32(dd1, sd1);

		m_0 &= ((emask >> 2) & 0x3);
		m_1 &= (emask & 0x3);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		dd0 = vis_bshuffle(sd0, dd0);
		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		dd1 = vis_bshuffle(sd1, dd1);

		i += 4;

		if (i < n) {

			emask = 0xFF << (4 - (n - i));

			s1 = (*sp++);
			sd0 = vis_faligndata(s0, s1);
			s0 = (*sp++);
			sd1 = vis_faligndata(s1, s0);

			m_0 = vis_fcmplt32(dd0, sd0);
			m_1 = vis_fcmplt32(dd1, sd1);

			m_0 &= ((emask >> 2) & 0x3);
			m_1 &= (emask & 0x3);

			vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
			dd0 = vis_bshuffle(sd0, dd0);
			vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
			dd1 = vis_bshuffle(sd1, dd1);
		}
	}

	pres[0] = dd0;
	pres[1] = dd1;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_S32_3_D1(
    mlib_d64 *pres,
    mlib_u8 *sa,
    mlib_s32 n)
{
	mlib_d64 *sp;
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 emask;
	mlib_d64 s0, s1;
	mlib_s32 i;

	sp = vis_alignaddr(sa, 0);

	dd0 = pres[0];
	dd1 = pres[1];
	dd2 = pres[2];

	s0 = (*sp++);

#pragma pipeloop(0)
	for (i = 0; i <= n - 12; i += 12) {
		mlib_d64 sd0, sd1, sd2, sd3, sd4, sd5;
		mlib_d64 lmax0, lmax1, lmax2;
		mlib_s32 m_0, m_1, m_2;

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

		m_0 = vis_fcmplt32(sd0, sd3);
		m_1 = vis_fcmplt32(sd1, sd4);
		m_2 = vis_fcmplt32(sd2, sd5);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		lmax0 = vis_bshuffle(sd3, sd0);

		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		lmax1 = vis_bshuffle(sd4, sd1);

		vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
		lmax2 = vis_bshuffle(sd5, sd2);

		m_0 = vis_fcmplt32(dd0, lmax0);
		m_1 = vis_fcmplt32(dd1, lmax1);
		m_2 = vis_fcmplt32(dd2, lmax2);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		dd0 = vis_bshuffle(lmax0, dd0);

		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		dd1 = vis_bshuffle(lmax1, dd1);

		vis_write_bmask(mlib_extrema_mask_s32[m_2], 0);
		dd2 = vis_bshuffle(lmax2, dd2);
	}

	if (i < n) {
		mlib_d64 sd0, sd1;
		mlib_s32 m_0, m_1;

		if ((n - i) < 4) {
			emask = 0xFF << (4 - (n - i));
		} else {
			emask = 0xFF;
		}

		s1 = (*sp++);
		sd0 = vis_faligndata(s0, s1);
		s0 = (*sp++);
		sd1 = vis_faligndata(s1, s0);

		m_0 = vis_fcmplt32(dd0, sd0);
		m_1 = vis_fcmplt32(dd1, sd1);

		m_0 &= ((emask >> 2) & 0x3);
		m_1 &= (emask & 0x3);

		vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
		dd0 = vis_bshuffle(sd0, dd0);
		vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
		dd1 = vis_bshuffle(sd1, dd1);

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

			m_0 = vis_fcmplt32(dd2, sd0);
			m_1 = vis_fcmplt32(dd0, sd1);

			m_0 &= ((emask >> 2) & 0x3);
			m_1 &= (emask & 0x3);

			vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
			dd2 = vis_bshuffle(sd0, dd2);
			vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
			dd0 = vis_bshuffle(sd1, dd0);

			i += 4;

			if (i < n) {
				emask = 0xFF << (4 - (n - i));

				s1 = (*sp++);
				sd0 = vis_faligndata(s0, s1);
				s0 = (*sp++);
				sd1 = vis_faligndata(s1, s0);

				m_0 = vis_fcmplt32(dd1, sd0);
				m_1 = vis_fcmplt32(dd2, sd1);

				m_0 &= ((emask >> 2) & 0x3);
				m_1 &= (emask & 0x3);

				vis_write_bmask(mlib_extrema_mask_s32[m_0], 0);
				dd1 = vis_bshuffle(sd0, dd1);
				vis_write_bmask(mlib_extrema_mask_s32[m_1], 0);
				dd2 = vis_bshuffle(sd1, dd2);
			}
		}
	}

	pres[0] = dd0;
	pres[1] = dd1;
	pres[2] = dd2;
}

/* *********************************************************** */

void
mlib_v_ImageMaximum_S32(
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

	dres[0] = dres[1] = dres[2] = vis_to_double_dup(0x80000000);

	if (slb == 4 * xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	if (nchan == 3) {
		for (j = 0; j < ysize; j++) {

			mlib_v_ImageMaximum_S32_3_D1(dres, sl, xsize);

			sl += slb;
		}

	} else {
		for (j = 0; j < ysize; j++) {

			mlib_v_ImageMaximum_S32_124_D1(dres, sl, xsize);

			sl += slb;
		}
	}

	*size = xsize;
}

/* *********************************************************** */
