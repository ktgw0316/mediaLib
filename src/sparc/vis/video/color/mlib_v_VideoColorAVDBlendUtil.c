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

#pragma ident	"@(#)mlib_v_VideoColorAVDBlendUtil.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *      [ZERO, ONE, DA, SA, OMDA, OMSA]
 *      blend two images with blend factors
 *
 * SYNOPSIS
 *      void mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *           [ZERO, ONE, DA, SA, OMDA, OMSA]
 *                                          (mlib_u32       * dst,
 *                                           const mlib_u32 * src1,
 *                                           const mlib_u32 * src2,
 *                                           mlib_s32       w,
 *                                           mlib_s32       h,
 *                                           mlib_s32       dlb,
 *                                           mlib_s32       slb1,
 *                                           mlib_s32       slb2)
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      w       image width
 *      h       image height
 *      dlb     linebytes for output image
 *      slb1    linebytes for 1st input image
 *      slb2    linebytes for 2nd input image
 *
 * DESCRIPTION
 *        dst = (src1 * src1_blend) + (src2 * src2_blend)
 *
 */

#include <mlib_algebra.h>
#include <mlib_v_ImageBlendTable.h>
#include <vis_proto.h>
#include <mlib_VideoColorAVDBlend_proto.h>

/* *********************************************************** */

#define	BLEND_VALIDATE                                          \
	mlib_u8 *psrc1, *psrc2, *pdst;                          \
	mlib_s32 i, j;                                          \
	                                                        \
	pdst = (mlib_u8 *)dst;                                  \
	psrc1 = (mlib_u8 *)src1;                                \
	psrc2 = (mlib_u8 *)src2

/* *********************************************************** */

#define	BLEND_VALIDATE_1                                        \
	mlib_u8 *psrc2, *pdst;                                  \
	mlib_s32 i, j;                                          \
	                                                        \
	pdst = (mlib_u8 *)dst;                                  \
	psrc2 = (mlib_u8 *)src2

/* *********************************************************** */

#define	MAX_SIZE	1024

/* *********************************************************** */

#define	BUFFER_SIZE                                             \
	if (w > MAX_SIZE && h == 1) {                           \
	    while ((w & 1) == 0 && w > MAX_SIZE) {              \
		w >>= 1;                                        \
		h *= 2;                                         \
	    }                                                   \
	    for (i = 3; w > MAX_SIZE; i += 2) {                 \
		while ((w % i) == 0 && w > MAX_SIZE) {          \
		    w /= i;                                     \
		    h *= i;                                     \
		}                                               \
	    }                                                   \
	    slb2 = dlb = w * 4;                                 \
	}

/* *********************************************************** */

#define	BLEND_ONE_DA(psrc1)                                              \
	alpha1 = vis_ld_u8(psrc1 + 8 * i + off);                         \
	alpha1 = vis_fexpand(vis_read_lo(alpha1));                       \
	res1 = vis_fmul8x16al(vis_read_hi(pix2), vis_read_lo(alpha1));   \
	res1 = vis_fpadd16(vis_fexpand(vis_read_hi(pix1)), res1);        \
	alpha2 = vis_ld_u8(psrc1 + 8 * i + 4 + off);                     \
	alpha2 = vis_fmul8x16al(vis_read_lo(alpha2), fone);              \
	res2 = vis_fmul8x16al(vis_read_lo(pix2), vis_read_lo(alpha2));   \
	res2 = vis_fpadd16(vis_fexpand(vis_read_lo(pix1)), res2)

/* *********************************************************** */

#define	BLEND_ONE_OMSA(psrc2)                                            \
	alpha1 = vis_ld_u8(psrc2 + 8 * i + off);                         \
	alpha1 = vis_fpsub16(d256, vis_fexpand(vis_read_lo(alpha1)));    \
	res1 = vis_fmul8x16al(vis_read_hi(pix2), vis_read_lo(alpha1));   \
	res1 = vis_fpadd16(vis_fexpand(vis_read_hi(pix1)), res1);        \
	alpha2 = vis_ld_u8(psrc2 + 8 * i + 4 + off);                     \
	alpha2 = vis_fmul8x16al(vis_read_lo(alpha2), fone);              \
	alpha2 = vis_fpsub16(d256, alpha2);                              \
	res2 = vis_fmul8x16al(vis_read_lo(pix2), vis_read_lo(alpha2));   \
	res2 = vis_fpadd16(vis_fexpand(vis_read_lo(pix1)), res2)

/* *********************************************************** */

#define	BLEND_ONE_OMDA(psrc1)                                            \
	alpha1 = vis_ld_u8(psrc1 + 8 * i + off);                         \
	alpha1 = vis_fpsub16(d256, vis_fexpand(vis_read_lo(alpha1)));    \
	res1 = vis_fmul8x16al(vis_read_hi(pix2), vis_read_lo(alpha1));   \
	res1 = vis_fpadd16(vis_fexpand(vis_read_hi(pix1)), res1);        \
	alpha2 = vis_ld_u8(psrc1 + 8 * i + 4 + off);                     \
	alpha2 = vis_fmul8x16al(vis_read_lo(alpha2), fone);              \
	alpha2 = vis_fpsub16(d256, alpha2);                              \
	res2 = vis_fmul8x16al(vis_read_lo(pix2), vis_read_lo(alpha2));   \
	res2 = vis_fpadd16(vis_fexpand(vis_read_lo(pix1)), res2)

/* *********************************************************** */

#define	BLEND_ONE_SA(psrc2)                                              \
	alpha1 = vis_ld_u8(psrc2 + 8 * i + off);                         \
	alpha1 = vis_fexpand(vis_read_lo(alpha1));                       \
	res1 = vis_fmul8x16al(vis_read_hi(pix2), vis_read_lo(alpha1));   \
	res1 = vis_fpadd16(vis_fexpand(vis_read_hi(pix1)), res1);        \
	alpha2 = vis_ld_u8(psrc2 + 8 * i + 4 + off);                     \
	alpha2 = vis_fmul8x16al(vis_read_lo(alpha2), fone);              \
	res2 = vis_fmul8x16al(vis_read_lo(pix2), vis_read_lo(alpha2));   \
	res2 = vis_fpadd16(vis_fexpand(vis_read_lo(pix1)), res2)

/* *********************************************************** */

#define	BLEND_ZERO_DA(src1, src2, res)                          \
	res = vis_ld_u8(src1);                                  \
	res = vis_fexpand(vis_read_lo(res));                    \
	res = vis_fmul8x16al(src2, vis_read_lo(res))

/* *********************************************************** */

#define	BLEND_ZERO_OMSA_hi(src, res)                            \
	res = vis_fpsub16(done, vis_fexpand(src));              \
	res = vis_fmul8x16au(src, vis_read_hi(res))

/* *********************************************************** */

#define	BLEND_ZERO_OMSA_PAIR_hi(src2, res)                      \
	BLEND_ZERO_OMSA_hi(vis_read_hi(src2), res);             \
	BLEND_ZERO_OMSA_hi(vis_read_lo(src2), res##1)

/* *********************************************************** */

#define	BLEND_ZERO_SA_hi(src, res)                              \
	res = vis_fexpand(src);                                 \
	res = vis_fmul8x16au(src, vis_read_hi(res))

/* *********************************************************** */

#define	BLEND_ZERO_SA_PAIR_hi(src2, res)                        \
	BLEND_ZERO_SA_hi(vis_read_hi(src2), res);               \
	BLEND_ZERO_SA_hi(vis_read_lo(src2), res##1)

/* *********************************************************** */

#define	BLEND_ZERO_DA(src1, src2, res)                          \
	res = vis_ld_u8(src1);                                  \
	res = vis_fexpand(vis_read_lo(res));                    \
	res = vis_fmul8x16al(src2, vis_read_lo(res))

/* *********************************************************** */

#define	BLEND_ZERO_DA_PAIR(src1, src2, res)                     \
	BLEND_ZERO_DA(src1, vis_read_hi(src2), res);            \
	BLEND_ZERO_DA(src1 + 4, vis_read_lo(src2), res##1)

/* *********************************************************** */

#define	UNALIGNLOAD_INC(pix2, dpsrc2)                           \
	pix2 = vis_write_hi(pix2, *(mlib_f32 *)dpsrc2);         \
	pix2 = vis_write_lo(pix2, *((mlib_f32 *)dpsrc2 + 1));   \
	dpsrc2++

/* *********************************************************** */

static void
mlib_VideoColorAdd(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 pix1, pix2;
	mlib_d64 pix1hi, pix1lo, pix2hi, pix2lo;
	mlib_d64 sum1, sum2;
	mlib_s32 ow = w;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {

			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				pix1hi = vis_fexpand(vis_read_hi(pix1));
				pix2hi = vis_fexpand(vis_read_hi(pix2));
				sum1 = vis_fpadd16(pix1hi, pix2hi);

				pix1lo = vis_fexpand(vis_read_lo(pix1));
				pix2lo = vis_fexpand(vis_read_lo(pix2));
				sum2 = vis_fpadd16(pix1lo, pix2lo);

				(*dpdst++) = vis_fpack16_pair(sum1, sum2);
			}

			if (w & 1) {
				pix1hi = vis_fexpand(*(mlib_f32 *)dpsrc1);
				pix2hi = vis_fexpand(*(mlib_f32 *)dpsrc2);
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				*(mlib_f32 *)dpdst = vis_fpack16(sum1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				pix1hi = vis_fexpand(*(mlib_f32 *)hpsrc1);
				pix2hi = vis_fexpand(*(mlib_f32 *)hpsrc2);
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				*(mlib_f32 *)hpdst = vis_fpack16(sum1);
				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				pix1hi = vis_fexpand(vis_read_hi(pix1));
				pix2hi = vis_fexpand(vis_read_hi(pix2));
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				pix1lo = vis_fexpand(vis_read_lo(pix1));
				pix2lo = vis_fexpand(vis_read_lo(pix2));
				sum2 = vis_fpadd16(pix1lo, pix2lo);
				(*dpdst++) = vis_fpack16_pair(sum1, sum2);
			}

			if (w & 1) {
				pix1hi = vis_fexpand(*(mlib_f32 *)(hpsrc1 +
					8 * i));
				pix2hi = vis_fexpand(*(mlib_f32 *)(hpsrc2 +
					8 * i));
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(sum1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorCopy(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		__mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src, w);
		dst += dlb / 4;
		src += slb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorCopy2(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 istop,
	mlib_s32 istart,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		__mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src, istop);
		__mlib_VectorCopy_S32((mlib_s32 *)dst + istart,
			(mlib_s32 *)src + istart, w - istart);
		dst += dlb / 4;
		src += slb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 alpha1, alpha2, alpha3, alpha4;
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 pix1, pix2, sum1, sum2;
	mlib_d64 res1, res2, res3, res4, res;
	mlib_s32 ow = w, off = 0;
	mlib_d64 *mlib_v_blend_alpha64 = (mlib_d64 *)mlib_table_blend_alpha_s32;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	for (j = 0; j < h; j++) {

		mlib_d64 s1l0, s1l1;
		mlib_s32 al;
		mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

		w = ow;

		dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
		al = (mlib_addr)hpdst - (mlib_addr)dpdst;
		dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
		dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
		s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

		if (al) {
			alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8(hpsrc1 +
				off)));
			alpha2 = mlib_v_blend_alpha64[256 - hpsrc2[off]];
			res1 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc1),
				vis_read_lo(alpha1));
			res2 = vis_fmul8x16(*(mlib_f32 *)(hpsrc2), alpha2);
			sum1 = vis_fpadd16(res1, res2);

			*(mlib_f32 *)(hpdst) = vis_fpack16(sum1);
			w--;
			hpsrc1 += 4;
			hpsrc2 += 4;
			hpdst += 4;
			dpdst++;
		}
#pragma pipeloop(0)
		for (i = 0; i < (w >> 1); i++) {
			s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
			UNALIGNLOAD_INC(pix2, dpsrc2);
			pix1 = vis_faligndata(s1l0, s1l1);
			s1l0 = s1l1;

			alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8(hpsrc1 +
				8 * i + off)));
			alpha2 = mlib_v_blend_alpha64[256 - hpsrc2[8 * i +
				off]];
			res1 = vis_fmul8x16al(vis_read_hi(pix1),
				vis_read_lo(alpha1));
			res2 = vis_fmul8x16(vis_read_hi(pix2), alpha2);
			sum1 = vis_fpadd16(res1, res2);

			alpha3 = vis_fexpand(vis_read_lo(vis_ld_u8(hpsrc1 +
				8 * i + 4 + off)));
			alpha4 = mlib_v_blend_alpha64[256 - hpsrc2[8 * i + 4 +
				off]];
			res3 = vis_fmul8x16al(vis_read_lo(pix1),
				vis_read_lo(alpha3));
			res4 = vis_fmul8x16(vis_read_lo(pix2), alpha4);
			sum2 = vis_fpadd16(res3, res4);

			res = vis_fpack16_pair(sum1, sum2);
			(*dpdst++) = res;
		}

		if (w & 1) {
			alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8(hpsrc1 +
				8 * i + off)));
			alpha2 = mlib_v_blend_alpha64[256 - hpsrc2[8 * i +
				off]];
			res1 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc1 + 8 * i),
				vis_read_lo(alpha1));
			res2 = vis_fmul8x16(*(mlib_f32 *)(hpsrc2 + 8 * i),
				alpha2);
			sum1 = vis_fpadd16(res1, res2);

			*(mlib_f32 *)(hpdst + 8 * i) = vis_fpack16(sum1);
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 alpha1, alpha2, alpha3, alpha4;

	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 pix1, pix2, sum1, sum2;
	mlib_d64 res1, res2, res3, res4, res;
	mlib_s32 ow = w, off = 0;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {

			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc1
					+ 8 * i + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc2
					+ 8 * i + off)));
				res1 = vis_fmul8x16al(vis_read_hi(pix1),
					vis_read_lo(alpha1));
				res2 = vis_fmul8x16al(vis_read_hi(pix2),
					vis_read_lo(alpha2));
				sum1 = vis_fpadd16(res1, res2);

				alpha3 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc1
					+ 8 * i + 4 + off)));
				alpha4 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc2
					+ 8 * i + 4 + off)));
				res3 = vis_fmul8x16al(vis_read_lo(pix1),
					vis_read_lo(alpha3));
				res4 = vis_fmul8x16al(vis_read_lo(pix2),
					vis_read_lo(alpha4));
				sum2 = vis_fpadd16(res3, res4);

				res = vis_fpack16_pair(sum1, sum2);
				(*dpdst++) = res;
			}

			if (w & 1) {

				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc1
					+ 8 * i + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc2
					+ 8 * i + off)));
				res1 = vis_fmul8x16al(*(mlib_f32 *)dpsrc1,
					vis_read_lo(alpha1));
				res2 = vis_fmul8x16al(*(mlib_f32 *)dpsrc2,
					vis_read_lo(alpha2));
				sum1 = vis_fpadd16(res1, res2);

				*(mlib_f32 *)dpdst = vis_fpack16(sum1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc1 + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc2 + off)));
				res1 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc1),
					vis_read_lo(alpha1));
				res2 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc2),
					vis_read_lo(alpha2));
				sum1 = vis_fpadd16(res1, res2);

				*(mlib_f32 *)(hpdst) = vis_fpack16(sum1);
				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc1 + 8 * i + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc2 + 8 * i + off)));
				res1 = vis_fmul8x16al(vis_read_hi(pix1),
					vis_read_lo(alpha1));
				res2 = vis_fmul8x16al(vis_read_hi(pix2),
					vis_read_lo(alpha2));
				sum1 = vis_fpadd16(res1, res2);

				alpha3 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc1 + 8 * i + 4 + off)));
				alpha4 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc2 + 8 * i + 4 + off)));
				res3 = vis_fmul8x16al(vis_read_lo(pix1),
					vis_read_lo(alpha3));
				res4 = vis_fmul8x16al(vis_read_lo(pix2),
					vis_read_lo(alpha4));
				sum2 = vis_fpadd16(res3, res4);

				res = vis_fpack16_pair(sum1, sum2);
				(*dpdst++) = res;
			}

			if (w & 1) {
				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc1 + 8 * i + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc2 + 8 * i + off)));
				res1 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc1 +
					8 * i), vis_read_lo(alpha1));
				res2 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc2 +
					8 * i), vis_read_lo(alpha2));
				sum1 = vis_fpadd16(res1, res2);

				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(sum1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 alpha1, alpha3, alpha2, alpha4;
	mlib_d64 pix1, pix2, sum1, sum2;
	mlib_d64 res1, res2, res3, res4;
	mlib_s32 ow = w, off = 0;
	mlib_d64 *mlib_v_blend_alpha64 = (mlib_d64 *)mlib_table_blend_alpha_s32;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {
			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				alpha1 = mlib_v_blend_alpha64[256 -
					psrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					psrc2[8 * i + off]];
				sum1 = vis_fmul8x16(vis_read_hi(pix1), alpha1);
				res1 = vis_fmul8x16(vis_read_hi(pix2), alpha2);
				res2 = vis_fpadd16(res1, sum1);

				alpha3 = mlib_v_blend_alpha64[256 -
					psrc1[8 * i + 4 + off]];
				alpha4 = mlib_v_blend_alpha64[256 -
					psrc2[8 * i + 4 + off]];
				sum2 = vis_fmul8x16(vis_read_lo(pix1), alpha3);
				res3 = vis_fmul8x16(vis_read_lo(pix2), alpha4);
				res4 = vis_fpadd16(res3, sum2);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {

				alpha1 = mlib_v_blend_alpha64[256 -
					psrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					psrc2[8 * i + off]];
				sum1 = vis_fmul8x16(*(mlib_f32 *)dpsrc1,
					alpha1);
				res1 = vis_fmul8x16(*(mlib_f32 *)dpsrc2,
					alpha2);
				res2 = vis_fpadd16(res1, sum1);

				*(mlib_f32 *)dpdst = vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = mlib_v_blend_alpha64[256 -
					hpsrc1[off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					hpsrc2[off]];
				sum1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc1),
					alpha1);
				res1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc2),
					alpha2);
				res2 = vis_fpadd16(res1, sum1);

				*(mlib_f32 *)(hpdst) = vis_fpack16(res2);
				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				alpha1 = mlib_v_blend_alpha64[256 -
					hpsrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					hpsrc2[8 * i + off]];
				sum1 = vis_fmul8x16(vis_read_hi(pix1), alpha1);
				res1 = vis_fmul8x16(vis_read_hi(pix2), alpha2);
				res2 = vis_fpadd16(res1, sum1);

				alpha3 = mlib_v_blend_alpha64[256 -
					hpsrc1[8 * i + 4 + off]];
				alpha4 = mlib_v_blend_alpha64[256 -
					hpsrc2[8 * i + 4 + off]];
				sum2 = vis_fmul8x16(vis_read_lo(pix1), alpha3);
				res3 = vis_fmul8x16(vis_read_lo(pix2), alpha4);
				res4 = vis_fpadd16(res3, sum2);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {
				alpha1 = mlib_v_blend_alpha64[256 -
					hpsrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					hpsrc2[8 * i + off]];
				sum1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc1 +
					8 * i), alpha1);
				res1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc2 +
					8 * i), alpha2);
				res2 = vis_fpadd16(res1, sum1);

				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 alpha1, alpha3, alpha2, alpha4;
	mlib_d64 pix1, pix2, sum1, sum2;
	mlib_d64 res1, res2, res3, res4;
	mlib_s32 ow = w, off = 0;
	mlib_d64 *mlib_v_blend_alpha64 = (mlib_d64 *)mlib_table_blend_alpha_s32;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {
			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				alpha1 = mlib_v_blend_alpha64[256 -
					psrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					psrc2[8 * i + off]];
				sum1 = vis_fmul8x16(vis_read_hi(pix2), alpha1);
				res1 = vis_fmul8x16(vis_read_hi(pix1), alpha2);
				res2 = vis_fpadd16(res1, sum1);

				alpha3 = mlib_v_blend_alpha64[256 -
					psrc1[8 * i + 4 + off]];
				alpha4 = mlib_v_blend_alpha64[256 -
					psrc2[8 * i + 4 + off]];
				sum2 = vis_fmul8x16(vis_read_lo(pix2), alpha3);
				res3 = vis_fmul8x16(vis_read_lo(pix1), alpha4);
				res4 = vis_fpadd16(res3, sum2);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {

				alpha1 = mlib_v_blend_alpha64[256 -
					psrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					psrc2[8 * i + off]];
				sum1 = vis_fmul8x16(*(mlib_f32 *)dpsrc2,
					alpha1);
				res1 = vis_fmul8x16(*(mlib_f32 *)dpsrc1,
					alpha2);
				res2 = vis_fpadd16(res1, sum1);

				*(mlib_f32 *)dpdst = vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = mlib_v_blend_alpha64[256 -
					hpsrc1[off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					hpsrc2[off]];
				sum1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc2),
					alpha1);
				res1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc1),
					alpha2);
				res2 = vis_fpadd16(res1, sum1);

				*(mlib_f32 *)(hpdst) = vis_fpack16(res2);
				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				alpha1 = mlib_v_blend_alpha64[256 -
					hpsrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					hpsrc2[8 * i + off]];
				sum1 = vis_fmul8x16(vis_read_hi(pix2), alpha1);
				res1 = vis_fmul8x16(vis_read_hi(pix1), alpha2);
				res2 = vis_fpadd16(res1, sum1);

				alpha3 = mlib_v_blend_alpha64[256 -
					hpsrc1[8 * i + 4 + off]];
				alpha4 = mlib_v_blend_alpha64[256 -
					hpsrc2[8 * i + 4 + off]];
				sum2 = vis_fmul8x16(vis_read_lo(pix2), alpha3);
				res3 = vis_fmul8x16(vis_read_lo(pix1), alpha4);
				res4 = vis_fpadd16(res3, sum2);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {
				alpha1 = mlib_v_blend_alpha64[256 -
					hpsrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha64[256 -
					hpsrc2[8 * i + off]];
				sum1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc2 +
					8 * i), alpha1);
				res1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc1 +
					8 * i), alpha2);
				res2 = vis_fpadd16(res1, sum1);

				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_f32 alpha1, alpha2;
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 pix1, pix2, pix1hi, pix1lo, pix2hi, pix2lo;
	mlib_d64 sum1, sum2;
	mlib_d64 res1, res2, res3, res4;
	mlib_s32 ow = w, off = 0;
	mlib_f32 *mlib_v_blend_alpha32 = (mlib_f32 *)mlib_table_blend_alpha_u32;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {

			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				alpha1 = mlib_v_blend_alpha32[psrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(vis_read_hi(pix1));
				pix2hi = vis_fexpand(vis_read_hi(pix2));
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpsub16(sum1, res1);

				alpha2 = mlib_v_blend_alpha32[psrc2[8 * i + 4 +
					off]];
				pix1lo = vis_fexpand(vis_read_lo(pix1));
				pix2lo = vis_fexpand(vis_read_lo(pix2));
				sum2 = vis_fpadd16(pix1lo, pix2lo);
				res3 = vis_fmul8x16(alpha2, sum2);
				res4 = vis_fpsub16(sum2, res3);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {

				alpha1 = mlib_v_blend_alpha32[psrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)dpsrc1);
				pix2hi = vis_fexpand(*(mlib_f32 *)dpsrc2);
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpsub16(sum1, res1);
				*(mlib_f32 *)dpdst = vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = mlib_v_blend_alpha32[hpsrc2[off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)(hpsrc1));
				pix2hi = vis_fexpand(*(mlib_f32 *)(hpsrc2));
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpsub16(sum1, res1);
				*(mlib_f32 *)(hpdst) = vis_fpack16(res2);
				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				alpha1 = mlib_v_blend_alpha32[hpsrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(vis_read_hi(pix1));
				pix2hi = vis_fexpand(vis_read_hi(pix2));
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpsub16(sum1, res1);

				alpha2 = mlib_v_blend_alpha32[hpsrc2[8 * i + 4 +
					off]];
				pix1lo = vis_fexpand(vis_read_lo(pix1));
				pix2lo = vis_fexpand(vis_read_lo(pix2));
				sum2 = vis_fpadd16(pix1lo, pix2lo);
				res3 = vis_fmul8x16(alpha2, sum2);
				res4 = vis_fpsub16(sum2, res3);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {
				alpha1 = mlib_v_blend_alpha32[hpsrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)(hpsrc1 +
					8 * i));
				pix2hi = vis_fexpand(*(mlib_f32 *)(hpsrc2 +
					8 * i));
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpsub16(sum1, res1);
				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 alpha2, alpha4, d256 = vis_to_double_dup(0x10001000);
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 pix1, pix2, sum1, sum2;
	mlib_d64 res1, res2, res3, res4;
	mlib_s32 ow = w, off = 0;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	for (j = 0; j < h; j++) {

		mlib_d64 s1l0, s1l1;
		mlib_s32 al;
		mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

		w = ow;
		dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
		al = (mlib_addr)hpdst - (mlib_addr)dpdst;
		dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
		dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
		s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

		if (al) {
			alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8(hpsrc2 +
				off)));
			res1 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc2),
				vis_read_lo(alpha2));
			sum1 = vis_fpsub16(d256, alpha2);
			res2 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc1),
				vis_read_lo(sum1));
			res2 = vis_fpadd16(res1, res2);
			*(mlib_f32 *)(hpdst) = vis_fpack16(res2);
			w--;
			hpsrc1 += 4;
			hpsrc2 += 4;
			hpdst += 4;
			dpdst++;
		}
#pragma pipeloop(0)
		for (i = 0; i < (w >> 1); i++) {
			s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
			UNALIGNLOAD_INC(pix2, dpsrc2);
			pix1 = vis_faligndata(s1l0, s1l1);
			s1l0 = s1l1;

			alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8(hpsrc2 +
				8 * i + off)));
			res1 = vis_fmul8x16al(vis_read_hi(pix2),
				vis_read_lo(alpha2));
			sum1 = vis_fpsub16(d256, alpha2);
			res2 = vis_fmul8x16al(vis_read_hi(pix1),
				vis_read_lo(sum1));
			res2 = vis_fpadd16(res1, res2);

			alpha4 = vis_fexpand(vis_read_lo(vis_ld_u8(hpsrc2 +
				8 * i + 4 + off)));
			res3 = vis_fmul8x16al(vis_read_lo(pix2),
				vis_read_lo(alpha4));
			sum2 = vis_fpsub16(d256, alpha4);
			res4 = vis_fmul8x16al(vis_read_lo(pix1),
				vis_read_lo(sum2));
			res4 = vis_fpadd16(res3, res4);

			(*dpdst++) = vis_fpack16_pair(res2, res4);
		}

		if (w & 1) {
			alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8(hpsrc2 +
				8 * i + off)));
			res1 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc2 + 8 * i),
				vis_read_lo(alpha2));
			sum1 = vis_fpsub16(d256, alpha2);
			res2 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc1 + 8 * i),
				vis_read_lo(sum1));
			res2 = vis_fpadd16(res1, res2);
			*(mlib_f32 *)(hpdst + 8 * i) = vis_fpack16(res2);
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 alpha1, alpha2, pix1, pix2, res1, res2;
	mlib_f32 fone = vis_to_float(0x1000);
	mlib_s32 ow = w, off = 0;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {
			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);
				BLEND_ONE_DA(psrc1);
				(*dpdst++) = vis_fpack16_pair(res1, res2);
			}

			if (w & 1) {
				alpha1 = vis_ld_u8(psrc1 + 8 * i + off);
				alpha1 = vis_fexpand(vis_read_lo(alpha1));
				res1 = vis_fmul8x16al(*((mlib_f32 *)dpsrc2),
					vis_read_lo(alpha1));
				res1 = vis_fpadd16(vis_fexpand(*((mlib_f32 *)
					dpsrc1)), res1);
				*((mlib_f32 *)dpdst) = vis_fpack16(res1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = vis_ld_u8(hpsrc1 + off);
				alpha1 = vis_fexpand(vis_read_lo(alpha1));
				res1 = vis_fmul8x16al(*((mlib_f32 *)(hpsrc2)),
					vis_read_lo(alpha1));
				res1 = vis_fpadd16(vis_fexpand(*((mlib_f32
					*)(hpsrc1))), res1);
				*((mlib_f32 *)(hpdst)) = vis_fpack16(res1);
				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				BLEND_ONE_DA(hpsrc1);
				(*dpdst++) = vis_fpack16_pair(res1, res2);
			}

			if (w & 1) {
				alpha1 = vis_ld_u8(hpsrc1 + 8 * i + off);
				alpha1 = vis_fexpand(vis_read_lo(alpha1));
				res1 = vis_fmul8x16al(*((mlib_f32 *)(hpsrc2 +
					8 * i)), vis_read_lo(alpha1));
				res1 = vis_fpadd16(vis_fexpand(*((mlib_f32
					*)(hpsrc1 + 8 * i))), res1);
				*((mlib_f32 *)(hpdst + 8 * i)) =
					vis_fpack16(res1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 alpha1, alpha2, pix1, pix2, res1, res2;
	mlib_d64 d256 = vis_to_double_dup(0x10001000);
	mlib_f32 fone = vis_to_float(0x1000);
	mlib_s32 ow = w, off = 0;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	for (j = 0; j < h; j++) {

		mlib_d64 s1l0, s1l1;
		mlib_s32 al;
		mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

		w = ow;
		dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
		al = (mlib_addr)hpdst - (mlib_addr)dpdst;
		dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
		dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
		s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

		if (al) {
			alpha1 = vis_ld_u8(hpsrc1 + off);
			alpha1 = vis_fexpand(vis_read_lo(alpha1));
			alpha1 = vis_fpsub16(d256, alpha1);
			res1 = vis_fmul8x16al(*((mlib_f32 *)(hpsrc2)),
				vis_read_lo(alpha1));
			res1 = vis_fpadd16(vis_fexpand(*((mlib_f32 *)(hpsrc1))),
				res1);
			*((mlib_f32 *)(hpdst)) = vis_fpack16(res1);
			w--;
			hpsrc1 += 4;
			hpsrc2 += 4;
			hpdst += 4;
			dpdst++;
		}
#pragma pipeloop(0)
		for (i = 0; i < (w >> 1); i++) {
			s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
			UNALIGNLOAD_INC(pix2, dpsrc2);
			pix1 = vis_faligndata(s1l0, s1l1);
			s1l0 = s1l1;

			BLEND_ONE_OMDA(hpsrc1);
			(*dpdst++) = vis_fpack16_pair(res1, res2);
		}

		if (w & 1) {
			alpha1 = vis_ld_u8(hpsrc1 + 8 * i + off);
			alpha1 = vis_fexpand(vis_read_lo(alpha1));
			alpha1 = vis_fpsub16(d256, alpha1);
			res1 = vis_fmul8x16al(*((mlib_f32 *)(hpsrc2 + 8 * i)),
				vis_read_lo(alpha1));
			res1 = vis_fpadd16(vis_fexpand(*((mlib_f32 *)(hpsrc1 +
				8 * i))), res1);
			*((mlib_f32 *)(hpdst + 8 * i)) = vis_fpack16(res1);
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 alpha1, alpha2, pix1, pix2, res1, res2;
	mlib_d64 d256 = vis_to_double_dup(0x10001000);
	mlib_f32 fone = vis_to_float(0x1000);
	mlib_s32 off = 0;
	mlib_s32 ow = w;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	for (j = 0; j < h; j++) {
		mlib_d64 s1l0, s1l1;
		mlib_s32 al;
		mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

		w = ow;

		dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
		al = (mlib_addr)hpdst - (mlib_addr)dpdst;
		dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
		dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
		s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

		if (al) {
			alpha1 = vis_ld_u8(hpsrc2 + off);
			alpha1 = vis_fmul8x16al(vis_read_lo(alpha1), fone);
			alpha1 = vis_fpsub16(d256, alpha1);
			res1 = vis_fmul8x16al(*((mlib_f32 *)(hpsrc2)),
				vis_read_lo(alpha1));
			res1 = vis_fpadd16(vis_fexpand(*((mlib_f32 *)(hpsrc1))),
				res1);
			*((mlib_f32 *)(hpdst)) = vis_fpack16(res1);

			w--;
			hpsrc1 += 4;
			hpsrc2 += 4;
			hpdst += 4;
			dpdst++;
		}
#pragma pipeloop(0)
		for (i = 0; i < (w >> 1); i++) {
			s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
			pix1 = vis_faligndata(s1l0, s1l1);
			s1l0 = s1l1;
			UNALIGNLOAD_INC(pix2, dpsrc2);

			BLEND_ONE_OMSA(hpsrc2);

			(*dpdst++) = vis_fpack16_pair(res1, res2);
		}

		if (w & 1) {
			alpha1 = vis_ld_u8(hpsrc2 + 8 * i + off);
			alpha1 = vis_fmul8x16al(vis_read_lo(alpha1), fone);
			alpha1 = vis_fpsub16(d256, alpha1);
			res1 = vis_fmul8x16al(*((mlib_f32 *)(hpsrc2 + 8 * i)),
				vis_read_lo(alpha1));
			res1 = vis_fpadd16(vis_fexpand(*((mlib_f32 *)(hpsrc1 +
				8 * i))), res1);
			*((mlib_f32 *)(hpdst + 8 * i)) = vis_fpack16(res1);
		}

		psrc1 += slb1;
		psrc2 += slb2;
		pdst += dlb;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorAdd(dst, src1, src2, w, h, dlb, slb1, slb2);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 alpha1, alpha2, pix1, pix2, res1, res2;
	mlib_f32 fone = vis_to_float(0x1000);
	mlib_s32 off = 0;
	mlib_s32 ow = w;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {
			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);
				BLEND_ONE_SA(psrc2);
				(*dpdst++) = vis_fpack16_pair(res1, res2);
			}

			if (w & 1) {
				alpha1 = vis_ld_u8(psrc2 + 8 * i + off);
				alpha1 = vis_fexpand(vis_read_lo(alpha1));
				res1 = vis_fmul8x16al(*((mlib_f32 *)dpsrc2),
					vis_read_lo(alpha1));
				res1 = vis_fpadd16(vis_fexpand(*((mlib_f32 *)
					dpsrc1)), res1);
				*((mlib_f32 *)dpdst) = vis_fpack16(res1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = vis_ld_u8(hpsrc2 + off);
				alpha1 = vis_fexpand(vis_read_lo(alpha1));
				res1 = vis_fmul8x16al(*((mlib_f32 *)(hpsrc2)),
					vis_read_lo(alpha1));
				res1 = vis_fpadd16(vis_fexpand(*((mlib_f32
					*)(hpsrc1))), res1);
				*((mlib_f32 *)(hpdst)) = vis_fpack16(res1);

				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				BLEND_ONE_SA(hpsrc2);

				(*dpdst++) = vis_fpack16_pair(res1, res2);
			}

			if (w & 1) {
				alpha1 = vis_ld_u8(hpsrc2 + 8 * i + off);
				alpha1 = vis_fexpand(vis_read_lo(alpha1));
				res1 = vis_fmul8x16al(*((mlib_f32 *)(hpsrc2 +
					8 * i)), vis_read_lo(alpha1));
				res1 = vis_fpadd16(vis_fexpand(*((mlib_f32
					*)(hpsrc1 + 8 * i))), res1);
				*((mlib_f32 *)(hpdst + 8 * i)) =
					vis_fpack16(res1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorCopy(dst, src1, w, h, dlb, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 alpha1, alpha2, alpha3, alpha4;
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 pix1, pix2;
	mlib_d64 sum1, sum2;
	mlib_d64 res1, res2, res3, res4, res;
	mlib_s32 ow = w, off = 0;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {

			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc1
					+ 8 * i + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc2
					+ 8 * i + off)));
				res1 = vis_fmul8x16al(vis_read_hi(pix1),
					vis_read_lo(alpha2));
				res2 = vis_fmul8x16al(vis_read_hi(pix2),
					vis_read_lo(alpha1));
				sum1 = vis_fpadd16(res1, res2);

				alpha3 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc1
					+ 8 * i + 4 + off)));
				alpha4 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc2
					+ 8 * i + 4 + off)));
				res3 = vis_fmul8x16al(vis_read_lo(pix1),
					vis_read_lo(alpha4));
				res4 = vis_fmul8x16al(vis_read_lo(pix2),
					vis_read_lo(alpha3));
				sum2 = vis_fpadd16(res3, res4);

				res = vis_fpack16_pair(sum1, sum2);
				(*dpdst++) = res;
			}

			if (w & 1) {

				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc1
					+ 8 * i + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8(psrc2
					+ 8 * i + off)));
				res1 = vis_fmul8x16al(*(mlib_f32 *)dpsrc1,
					vis_read_lo(alpha2));
				res2 = vis_fmul8x16al(*(mlib_f32 *)dpsrc2,
					vis_read_lo(alpha1));
				sum1 = vis_fpadd16(res1, res2);

				*(mlib_f32 *)dpdst = vis_fpack16(sum1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc1 + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc2 + off)));
				res1 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc1),
					vis_read_lo(alpha2));
				res2 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc2),
					vis_read_lo(alpha1));
				sum1 = vis_fpadd16(res1, res2);
				*(mlib_f32 *)(hpdst) = vis_fpack16(sum1);

				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc1 + 8 * i + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc2 + 8 * i + off)));
				res1 = vis_fmul8x16al(vis_read_hi(pix1),
					vis_read_lo(alpha2));
				res2 = vis_fmul8x16al(vis_read_hi(pix2),
					vis_read_lo(alpha1));
				sum1 = vis_fpadd16(res1, res2);

				alpha3 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc1 + 8 * i + 4 + off)));
				alpha4 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc2 + 8 * i + 4 + off)));
				res3 = vis_fmul8x16al(vis_read_lo(pix1),
					vis_read_lo(alpha4));
				res4 = vis_fmul8x16al(vis_read_lo(pix2),
					vis_read_lo(alpha3));
				sum2 = vis_fpadd16(res3, res4);

				res = vis_fpack16_pair(sum1, sum2);
				(*dpdst++) = res;
			}

			if (w & 1) {
				alpha1 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc1 + 8 * i + off)));
				alpha2 = vis_fexpand(vis_read_lo(vis_ld_u8
					(hpsrc2 + 8 * i + off)));
				res1 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc1 +
					8 * i), vis_read_lo(alpha2));
				res2 = vis_fmul8x16al(*(mlib_f32 *)(hpsrc2 +
					8 * i), vis_read_lo(alpha1));
				sum1 = vis_fpadd16(res1, res2);

				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(sum1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_f32 alpha2, alpha4;
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 alpha1, alpha3;
	mlib_d64 pix1, pix2;
	mlib_d64 pix1hi, pix1lo;
	mlib_d64 sum1, sum2;
	mlib_d64 res1, res2, res3, res4;
	mlib_s32 ow = w, off = 0;
	mlib_d64 *mlib_v_blend_alpha64 = (mlib_d64 *)mlib_table_blend_alpha_s32;
	mlib_f32 *mlib_v_blend_alpha32 = (mlib_f32 *)mlib_table_blend_alpha_u32;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {

			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				alpha1 = mlib_v_blend_alpha64[256 -
					psrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha32[psrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(vis_read_hi(pix1));
				sum1 = vis_fmul8x16(vis_read_hi(pix2), alpha1);
				res1 = vis_fmul8x16(alpha2, pix1hi);
				res2 = vis_fpadd16(res1, sum1);

				alpha3 = mlib_v_blend_alpha64[256 -
					psrc1[8 * i + 4 + off]];
				alpha4 = mlib_v_blend_alpha32[psrc2[8 * i + 4 +
					off]];
				pix1lo = vis_fexpand(vis_read_lo(pix1));
				sum2 = vis_fmul8x16(vis_read_lo(pix2), alpha3);
				res3 = vis_fmul8x16(alpha4, pix1lo);
				res4 = vis_fpadd16(res3, sum2);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {

				alpha1 = mlib_v_blend_alpha64[256 -
					psrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha32[psrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)dpsrc1);
				sum1 = vis_fmul8x16(*(mlib_f32 *)dpsrc2,
					alpha1);
				res1 = vis_fmul8x16(alpha2, pix1hi);
				res2 = vis_fpadd16(res1, sum1);

				*(mlib_f32 *)dpdst = vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;
			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = mlib_v_blend_alpha64[256 -
					hpsrc1[off]];
				alpha2 = mlib_v_blend_alpha32[hpsrc2[off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)(hpsrc1));
				sum1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc2),
					alpha1);
				res1 = vis_fmul8x16(alpha2, pix1hi);
				res2 = vis_fpadd16(res1, sum1);

				*(mlib_f32 *)hpdst = vis_fpack16(res2);
				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				alpha1 = mlib_v_blend_alpha64[256 -
					hpsrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha32[hpsrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(vis_read_hi(pix1));
				sum1 = vis_fmul8x16(vis_read_hi(pix2), alpha1);
				res1 = vis_fmul8x16(alpha2, pix1hi);
				res2 = vis_fpadd16(res1, sum1);

				alpha3 = mlib_v_blend_alpha64[256 -
					hpsrc1[8 * i + 4 + off]];
				alpha4 = mlib_v_blend_alpha32[hpsrc2[8 * i + 4 +
					off]];
				pix1lo = vis_fexpand(vis_read_lo(pix1));
				sum2 = vis_fmul8x16(vis_read_lo(pix2), alpha3);
				res3 = vis_fmul8x16(alpha4, pix1lo);
				res4 = vis_fpadd16(res3, sum2);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {
				alpha1 = mlib_v_blend_alpha64[256 -
					hpsrc1[8 * i + off]];
				alpha2 = mlib_v_blend_alpha32[hpsrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)(hpsrc1 +
					8 * i));
				sum1 = vis_fmul8x16(*(mlib_f32 *)(hpsrc2 +
					8 * i), alpha1);
				res1 = vis_fmul8x16(alpha2, pix1hi);
				res2 = vis_fpadd16(res1, sum1);

				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_f32 alpha1, alpha2;
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 pix1, pix2;
	mlib_d64 pix1hi, pix1lo;
	mlib_d64 pix2hi, pix2lo;
	mlib_d64 sum1, sum2;
	mlib_d64 res1, res2, res3, res4;
	mlib_s32 ow = w, off = 0;
	mlib_f32 *mlib_v_blend_alpha32 = (mlib_f32 *)mlib_table_blend_alpha_u32;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {

		for (j = 0; j < h; j++) {

			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				alpha1 = mlib_v_blend_alpha32[psrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(vis_read_hi(pix1));
				pix2hi = vis_fexpand(vis_read_hi(pix2));
				sum1 = vis_fpsub16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpadd16(res1, pix2hi);

				alpha2 = mlib_v_blend_alpha32[psrc2[8 * i + 4 +
					off]];
				pix1lo = vis_fexpand(vis_read_lo(pix1));
				pix2lo = vis_fexpand(vis_read_lo(pix2));
				sum2 = vis_fpsub16(pix1lo, pix2lo);
				res3 = vis_fmul8x16(alpha2, sum2);
				res4 = vis_fpadd16(res3, pix2lo);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {

				alpha1 = mlib_v_blend_alpha32[psrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)dpsrc1);
				pix2hi = vis_fexpand(*(mlib_f32 *)dpsrc2);
				sum1 = vis_fpsub16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpadd16(res1, pix2hi);
				*(mlib_f32 *)dpdst = vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = mlib_v_blend_alpha32[hpsrc2[off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)hpsrc1);
				pix2hi = vis_fexpand(*(mlib_f32 *)hpsrc2);
				sum1 = vis_fpsub16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpadd16(res1, pix2hi);
				*((mlib_f32 *)hpdst) = vis_fpack16(res2);
				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;
				alpha1 = mlib_v_blend_alpha32[hpsrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(vis_read_hi(pix1));
				pix2hi = vis_fexpand(vis_read_hi(pix2));
				sum1 = vis_fpsub16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpadd16(res1, pix2hi);

				alpha2 = mlib_v_blend_alpha32[hpsrc2[8 * i + 4 +
					off]];
				pix1lo = vis_fexpand(vis_read_lo(pix1));
				pix2lo = vis_fexpand(vis_read_lo(pix2));
				sum2 = vis_fpsub16(pix1lo, pix2lo);
				res3 = vis_fmul8x16(alpha2, sum2);
				res4 = vis_fpadd16(res3, pix2lo);

				(*dpdst++) = vis_fpack16_pair(res2, res4);
			}

			if (w & 1) {
				alpha1 = mlib_v_blend_alpha32[hpsrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)(hpsrc1 +
					8 * i));
				pix2hi = vis_fexpand(*(mlib_f32 *)(hpsrc2 +
					8 * i));
				sum1 = vis_fpsub16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				res2 = vis_fpadd16(res1, pix2hi);
				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(res2);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_f32 alpha1, alpha2;
	mlib_d64 *dpdst, *dpsrc1, *dpsrc2;
	mlib_d64 pix1, pix2;
	mlib_d64 pix1hi, pix1lo, pix2hi, pix2lo;
	mlib_d64 sum1, sum2, res1, res2;
	mlib_s32 ow = w, off = 0;
	mlib_f32 *mlib_v_blend_alpha32 = (mlib_f32 *)mlib_table_blend_alpha_u32;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc1 & 7) == 0 && ((mlib_addr)psrc2 & 7) == 0 &&
		((mlib_addr)pdst & 7) == 0 && (dlb & 7) == 0 &&
		(slb1 & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {

			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {

				pix1 = (*dpsrc1++);
				pix2 = (*dpsrc2++);

				alpha1 = mlib_v_blend_alpha32[psrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(vis_read_hi(pix1));
				pix2hi = vis_fexpand(vis_read_hi(pix2));
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);

				alpha2 = mlib_v_blend_alpha32[psrc2[8 * i + 4 +
					off]];
				pix1lo = vis_fexpand(vis_read_lo(pix1));
				pix2lo = vis_fexpand(vis_read_lo(pix2));
				sum2 = vis_fpadd16(pix1lo, pix2lo);
				res2 = vis_fmul8x16(alpha2, sum2);

				(*dpdst++) = vis_fpack16_pair(res1, res2);
			}

			if (w & 1) {
				alpha1 = mlib_v_blend_alpha32[psrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)dpsrc1);
				pix2hi = vis_fexpand(*(mlib_f32 *)dpsrc2);
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				*(mlib_f32 *)dpdst = vis_fpack16(res1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {

			mlib_d64 s1l0, s1l1;
			mlib_s32 al;
			mlib_u8 *hpsrc1 = psrc1, *hpsrc2 = psrc2, *hpdst = pdst;

			w = ow;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & (~7));
			al = (mlib_addr)hpdst - (mlib_addr)dpdst;
			dpsrc2 = (mlib_d64 *)(hpsrc2 + al);
			dpsrc1 = (mlib_d64 *)vis_alignaddr(hpsrc1, al);
			s1l0 = vis_ld_d64_nf(dpsrc1); dpsrc1++;

			if (al) {
				alpha1 = mlib_v_blend_alpha32[hpsrc2[off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)hpsrc1);
				pix2hi = vis_fexpand(*(mlib_f32 *)hpsrc2);
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				*(mlib_f32 *)hpdst = vis_fpack16(res1);
				w--;
				hpsrc1 += 4;
				hpsrc2 += 4;
				hpdst += 4;
				dpdst++;
			}
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				s1l1 = vis_ld_d64_nf(dpsrc1); dpsrc1++;
				UNALIGNLOAD_INC(pix2, dpsrc2);
				pix1 = vis_faligndata(s1l0, s1l1);
				s1l0 = s1l1;

				alpha1 = mlib_v_blend_alpha32[hpsrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(vis_read_hi(pix1));
				pix2hi = vis_fexpand(vis_read_hi(pix2));
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);

				alpha2 = mlib_v_blend_alpha32[hpsrc2[8 * i + 4 +
					off]];
				pix1lo = vis_fexpand(vis_read_lo(pix1));
				pix2lo = vis_fexpand(vis_read_lo(pix2));
				sum2 = vis_fpadd16(pix1lo, pix2lo);
				res2 = vis_fmul8x16(alpha2, sum2);

				(*dpdst++) = vis_fpack16_pair(res1, res2);
			}

			if (w & 1) {
				alpha1 = mlib_v_blend_alpha32[hpsrc2[8 * i +
					off]];
				pix1hi = vis_fexpand(*(mlib_f32 *)(hpsrc1 +
					8 * i));
				pix2hi = vis_fexpand(*(mlib_f32 *)(hpsrc2 +
					8 * i));
				sum1 = vis_fpadd16(pix1hi, pix2hi);
				res1 = vis_fmul8x16(alpha1, sum1);
				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(res1);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpsrc2, *dpdst;
	mlib_d64 dsrc2, tmp0, tmp1, dres, dres1;

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc2 & 7) == 0 && ((mlib_addr)pdst & 7) == 0 &&
		(dlb & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {
			mlib_u8 *palpha = psrc1;

			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				dsrc2 = (*dpsrc2++);
				BLEND_ZERO_DA_PAIR(palpha, dsrc2, dres);
				(*dpdst++) = vis_fpack16_pair(dres, dres1);
				palpha += 8;
			}

			if (w & 1) {
				mlib_f32 fsrc2 = *(mlib_f32 *)dpsrc2;

				BLEND_ZERO_DA(palpha, fsrc2, dres);
				*(mlib_f32 *)dpdst = vis_fpack16(dres);
			}

			psrc2 += slb2;
			pdst += dlb;
			psrc1 += slb1;
		}
	} else {
		mlib_s32 ow = w;

		for (j = 0; j < h; j++) {
			mlib_u8 *palpha = psrc1;
			mlib_u8 *hpsrc2 = psrc2, *hpdst = pdst;
			mlib_s32 al;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & ~7);
			al = hpdst - (mlib_u8 *)dpdst;
			w = ow;

			if (al) {
				mlib_f32 fsrc2 = *(mlib_f32 *)hpsrc2;

				BLEND_ZERO_DA(palpha, fsrc2, dres);
				*(mlib_f32 *)hpdst = vis_fpack16(dres);
				palpha += 4;
				hpdst += 4;
				hpsrc2 += 4;
				dpdst++;
				w--;
			}

			dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, al);
			tmp0 = vis_ld_d64_nf(dpsrc2); dpsrc2++;
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				tmp1 = vis_ld_d64_nf(dpsrc2); dpsrc2++;
				dsrc2 = vis_faligndata(tmp0, tmp1);
				tmp0 = tmp1;

				BLEND_ZERO_DA_PAIR(palpha, dsrc2, dres);
				(*dpdst++) = vis_fpack16_pair(dres, dres1);
				palpha += 8;
			}

			if (w & 1) {
				mlib_f32 fsrc2 = *(mlib_f32 *)(hpsrc2 + 8 * i);

				BLEND_ZERO_DA(palpha, fsrc2, dres);
				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(dres);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpsrc2, *dpdst;
	mlib_d64 dsrc2, tmp0, tmp1, dres, dres1;
	mlib_d64 done = vis_to_double_dup(0x10001000);

	BLEND_VALIDATE;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc2 & 7) == 0 && ((mlib_addr)pdst & 7) == 0 &&
		(dlb & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {
			mlib_u8 *palpha = psrc1;

			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;

#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				tmp1 = (*dpsrc2++);
				dres = vis_ld_u8(palpha);
				dres = vis_fpsub16(done,
					vis_fexpand(vis_read_lo(dres)));
				dres = vis_fmul8x16al(vis_read_hi(tmp1),
					vis_read_lo(dres));
				dres1 = vis_ld_u8(palpha + 4);
				dres1 = vis_fpsub16(done,
					vis_fexpand(vis_read_lo(dres1)));
				dres1 = vis_fmul8x16al(vis_read_lo(tmp1),
					vis_read_lo(dres1));
				(*dpdst++) = vis_fpack16_pair(dres, dres1);
				palpha += 8;
			}

			if (w & 1) {
				mlib_f32 fsrc2 = *(mlib_f32 *)dpsrc2;

				dres = vis_ld_u8(palpha);
				dres = vis_fpsub16(done,
					vis_fexpand(vis_read_lo(dres)));
				dres = vis_fmul8x16al(fsrc2, vis_read_lo(dres));
				*(mlib_f32 *)dpdst = vis_fpack16(dres);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		mlib_s32 ow = w;

		for (j = 0; j < h; j++) {
			mlib_u8 *palpha = psrc1;
			mlib_u8 *hpsrc2 = psrc2, *hpdst = pdst;
			mlib_s32 al;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & ~7);
			al = hpdst - (mlib_u8 *)dpdst;
			w = ow;

			if (al) {
				mlib_f32 fsrc2 = *(mlib_f32 *)hpsrc2;

				dres = vis_ld_u8(palpha);
				dres = vis_fpsub16(done,
					vis_fexpand(vis_read_lo(dres)));
				dres = vis_fmul8x16al(fsrc2, vis_read_lo(dres));
				*(mlib_f32 *)hpdst = vis_fpack16(dres);
				palpha += 4;
				hpdst += 4;
				hpsrc2 += 4;
				dpdst++;
				w--;
			}

			dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, al);
			tmp0 = vis_ld_d64_nf(dpsrc2); dpsrc2++;
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				tmp1 = vis_ld_d64_nf(dpsrc2); dpsrc2++;
				dsrc2 = vis_faligndata(tmp0, tmp1);
				tmp0 = tmp1;

				dres = vis_ld_u8(palpha);
				dres = vis_fpsub16(done,
					vis_fexpand(vis_read_lo(dres)));
				dres = vis_fmul8x16al(vis_read_hi(dsrc2),
					vis_read_lo(dres));
				dres1 = vis_ld_u8(palpha + 4);
				dres1 = vis_fpsub16(done,
					vis_fexpand(vis_read_lo(dres1)));
				dres1 = vis_fmul8x16al(vis_read_lo(dsrc2),
					vis_read_lo(dres1));
				(*dpdst++) = vis_fpack16_pair(dres, dres1);

				palpha += 8;
			}

			if (w & 1) {
				mlib_f32 fsrc2 = *(mlib_f32 *)(hpsrc2 + 8 * i);

				dres = vis_ld_u8(palpha);
				dres = vis_fpsub16(done,
					vis_fexpand(vis_read_lo(dres)));
				dres = vis_fmul8x16al(fsrc2, vis_read_lo(dres));
				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(dres);
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_OMSA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpsrc2;
	mlib_d64 done = vis_to_double_dup(0x10001000);
	mlib_d64 dsrc2, tmp0, tmp1, dres, dres1;
	mlib_u8 *psrc2_row, *pdst_row, *buf;
	mlib_s32 shag;

	BLEND_VALIDATE_1;

	vis_write_gsr(3 << 3);
	BUFFER_SIZE;

	buf = pdst;
	shag = dlb;

	for (i = 0; i < h; i++) {

		psrc2_row = psrc2 + i * slb2;
		pdst_row = buf + i * shag;

		if (j = ((mlib_addr)pdst_row >> 2) & 1) {
			dpsrc2 = vis_alignaddr(psrc2_row, 0);
			dsrc2 = vis_faligndata(vis_ld_d64_nf(dpsrc2),
				vis_ld_d64_nf(dpsrc2 + 1));

			BLEND_ZERO_OMSA_hi(vis_read_hi(dsrc2), dres);
			((mlib_f32 *)pdst_row)[0] = vis_fpack16(dres);
			psrc2_row += 4;
			pdst_row += 4;
		}

		dpsrc2 = vis_alignaddr(psrc2_row, 0);
		tmp1 = vis_ld_d64_nf(dpsrc2);

		if (((mlib_addr)psrc2_row & 7) == 0) {
#pragma pipeloop(0)
			for (; j <= (w - 2); j += 2) {
				BLEND_ZERO_OMSA_PAIR_hi(tmp1, dres);
				tmp1 = vis_ld_d64_nf(dpsrc2 + 1);
				((mlib_d64 *)pdst_row)[0] =
					vis_fpack16_pair(dres, dres1);

				dpsrc2++;
				pdst_row += 8;
			}
		} else {
#pragma pipeloop(0)
			for (; j <= (w - 2); j += 2) {
				tmp0 = tmp1;
				tmp1 = vis_ld_d64_nf(dpsrc2 + 1);
				dsrc2 = vis_faligndata(tmp0, tmp1);

				BLEND_ZERO_OMSA_PAIR_hi(dsrc2, dres);
				((mlib_d64 *)pdst_row)[0] =
					vis_fpack16_pair(dres, dres1);

				dpsrc2++;
				pdst_row += 8;
			}
		}

		if (j < w) {
			dsrc2 = vis_faligndata(tmp1, vis_ld_d64_nf(dpsrc2 + 1));
			BLEND_ZERO_OMSA_hi(vis_read_hi(dsrc2), dres);
			((mlib_f32 *)pdst_row)[0] = vis_fpack16(dres);
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorCopy(dst, src2, w, h, dlb, slb2);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_d64 *dpsrc2, *dpdst;
	mlib_d64 dsrc2, tmp0, tmp1, dres, dres1;

	BLEND_VALIDATE_1;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)psrc2 & 7) == 0 && ((mlib_addr)pdst & 7) == 0 &&
		(dlb & 7) == 0 && (slb2 & 7) == 0) {
		for (j = 0; j < h; j++) {
			dpsrc2 = (mlib_d64 *)psrc2;
			dpdst = (mlib_d64 *)pdst;
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				dsrc2 = (*dpsrc2++);
				BLEND_ZERO_SA_PAIR_hi(dsrc2, dres);
				(*dpdst++) = vis_fpack16_pair(dres, dres1);
			}

			if (w & 1) {
				mlib_f32 fsrc2 = *(mlib_f32 *)dpsrc2;

				BLEND_ZERO_SA_hi(fsrc2, dres);
				*(mlib_f32 *)dpdst = vis_fpack16(dres);
			}

			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		mlib_s32 ow = w;

		for (j = 0; j < h; j++) {
			mlib_u8 *hpsrc2 = psrc2, *hpdst = pdst;
			mlib_s32 al;

			dpdst = (mlib_d64 *)((mlib_addr)hpdst & ~7);
			al = hpdst - (mlib_u8 *)dpdst;
			w = ow;

			if (al) {
				mlib_f32 fsrc2 = *(mlib_f32 *)hpsrc2;

				BLEND_ZERO_SA_hi(fsrc2, dres);
				*(mlib_f32 *)hpdst = vis_fpack16(dres);
				hpdst += 4;
				hpsrc2 += 4;
				dpdst++;
				w--;
			}

			dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, al);
			tmp0 = vis_ld_d64_nf(dpsrc2); dpsrc2++;
#pragma pipeloop(0)
			for (i = 0; i < (w >> 1); i++) {
				tmp1 = vis_ld_d64_nf(dpsrc2); dpsrc2++;
				dsrc2 = vis_faligndata(tmp0, tmp1);
				tmp0 = tmp1;

				BLEND_ZERO_SA_PAIR_hi(dsrc2, dres);
				(*dpdst++) = vis_fpack16_pair(dres, dres1);
			}

			if (w & 1) {
				mlib_f32 fsrc2 = *(mlib_f32 *)(hpsrc2 + 8 * i);

				BLEND_ZERO_SA_hi(fsrc2, dres);
				*(mlib_f32 *)(hpdst + 8 * i) =
					vis_fpack16(dres);
			}

			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		__mlib_VectorZero_S32((mlib_s32 *)dst, w);
		dst += dlb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_SA_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_OMSA_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_SA_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_OMSA_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_DA_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_SA_OMDA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_OMDA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_OMDA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_DA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_DA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */
