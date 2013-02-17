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
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageZoom_BL.c	9.7	08/10/29 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslate(), internal pixels.
 *
 */

#include <mlib_s_ImageZoom.h>

/* *********************************************************** */

#define	ZOOM_D()                            \
	y0 = _mm_mulhi_epu16(rdy, y0);      \
	y1 = _mm_mulhi_epu16(ddy, y1);      \
	dd0 = _mm_add_epi16(y0, y1);        \
	dd1 = _mm_add_epi16(dd0, k05);      \
	dd2 = _mm_srli_epi16(dd1, 8);       \
	dd3 = _mm_packus_epi16(dd2, dd2)

/* *********************************************************** */

mlib_status
mlib_s_ImageZoom_BL_U8_1(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1, *buffz;
	void *buff = buff_arr;
	mlib_u8 *tdp;
	mlib_s32 k, width8, size, y_step, next_step = 2;
	__m128i ddx_start, ddy, rdy, dx_step, dy_step;
	__m128i mask_ffff, k05;

	width8 = (width + 7) / 8;

	size = (3 * width8 + 1) * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = (__m128i *)((mlib_addr)buff & ~15) + 1;
	buff1 = buff0 + width8;
	buffz = buff1 + width8;

	mask_ffff = _mm_set1_epi16(0xFFFF);
	k05 = _mm_set1_epi16(1 << 7);

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (k = 0; k < 8; k++) {
		((mlib_s16 *)&ddx_start)[k] = x;
		x += dx;
	}
	dx_step = _mm_set1_epi16(8 * dx);
	ddy = _mm_set1_epi16(y);
	rdy = _mm_sub_epi16(mask_ffff, ddy);
	dy_step = _mm_set1_epi16(dy);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0, k = 0; i < width / 16; i++, k += 2) {
				__m128i y0, y1, y2, y3;
				__m128i dd0, dd1, dd2, dd3, dd4;
				y0 = buff0[k];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = buff1[k];
				y1 = _mm_mulhi_epu16(ddy, y1);
				y2 = buff0[k + 1];
				y2 = _mm_mulhi_epu16(rdy, y2);
				y3 = buff1[k + 1];
				y3 = _mm_mulhi_epu16(ddy, y3);
				dd0 = _mm_add_epi16(y0, y1);
				dd1 = _mm_add_epi16(dd0, k05);
				dd2 = _mm_srli_epi16(dd1, 8);

				dd0 = _mm_add_epi16(y2, y3);
				dd1 = _mm_add_epi16(dd0, k05);
				dd3 = _mm_srli_epi16(dd1, 8);

				dd4 = _mm_packus_epi16(dd2, dd3);
				_mm_storeu_si128((void *)tdp, dd4);
				tdp += 16;
			}
			i *= 2;
			if (i < width / 8) {
				__m128i y0, y1;
				__m128i dd0, dd1, dd2, dd3;
				y0 = buff0[i];
				y1 = buff1[i];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = _mm_mulhi_epu16(ddy, y1);
				dd0 = _mm_add_epi16(y0, y1);
				dd1 = _mm_add_epi16(dd0, k05);
				dd2 = _mm_srli_epi16(dd1, 8);
				dd3 = _mm_packus_epi16(dd2, dd2);
				_mm_storel_epi64((void *)tdp, dd3);
				tdp += 8;
				i++;
			}

			if (width & 7) {
				__m128i y0, y1;
				__m128i dd0, dd1, dd2, dd3;
				y0 = buff0[i];
				y1 = buff1[i];
				ZOOM_D();
				for (i = 0; i < (width & 7); i++) {
					tdp[i] = ((mlib_u8 *)&dd3)[i];
				}
			}
		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;

			ddx = ddx_start;
			rdx = _mm_sub_epi16(mask_ffff, ddx);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width / 8; i++) {
				__m128i i0, i1, i2, i3;
				__m128i v2, v3, y0, y1, z0, z1, dd;
				__m128i s2, s3;

				y0 = buff0[i];
				z0 = _mm_mulhi_epu16(rdy, y0);

				mlib_u8 *tsp2 = tsp + src_stride;
				mlib_u8 *sp0 = tsp2 + (x >> MLIB_SHIFT);
				mlib_u8 *sp1 = tsp2 + ((x + dx) >> MLIB_SHIFT);
				mlib_u8 *sp2 =
				    tsp2 + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp3 =
				    tsp2 + ((x + 3 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp4 =
				    tsp2 + ((x + 4 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp5 =
				    tsp2 + ((x + 5 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp6 =
				    tsp2 + ((x + 6 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp7 =
				    tsp2 + ((x + 7 * dx) >> MLIB_SHIFT);
				x += 8 * dx;

				i0 = _mm_cvtsi32_si128(
				    sp0[0] | (sp1[0] << 8) |
				    (sp2[0] << 16) | (sp3[0] << 24));
				i1 = _mm_cvtsi32_si128(
				    sp4[0] | (sp5[0] << 8) |
				    (sp6[0] << 16) | (sp7[0] << 24));
				s2 = _mm_unpacklo_epi32(i0, i1);
				s2 = _mm_unpacklo_epi8(_mm_setzero_si128(), s2);
				v2 = _mm_mulhi_epu16(s2, rdx);

				i2 = _mm_cvtsi32_si128(
				    sp0[1] | (sp1[1] << 8) |
				    (sp2[1] << 16) | (sp3[1] << 24));
				i3 = _mm_cvtsi32_si128(
				    sp4[1] | (sp5[1] << 8) |
				    (sp6[1] << 16) | (sp7[1] << 24));
				s3 = _mm_unpacklo_epi32(i2, i3);

				s3 = _mm_unpacklo_epi8(_mm_setzero_si128(), s3);
				v3 = _mm_mulhi_epu16(s3, ddx);

				ddx = _mm_add_epi16(ddx, dx_step);

				y1 = _mm_add_epi16(v2, v3);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				buffz[i] = dd;
				buff1[i] = y1;
				rdx = _mm_sub_epi16(mask_ffff, ddx);
			}
			if (width & 7) {
				__m128i i0, i1, i2, i3;
				__m128i v2, v3, y0, y1, z0, z1, dd;
				__m128i s2, s3;
				mlib_s32 leftover = width & 7;

				y0 = buff0[i];
				z0 = _mm_mulhi_epu16(rdy, y0);

				mlib_u8 *tsp2 = tsp + src_stride;
				mlib_u8 *sp0 = tsp2 + (x >> MLIB_SHIFT);
				mlib_u8 *sp1 = tsp2 + ((x + dx) >> MLIB_SHIFT);
				mlib_u8 *sp2 =
				    tsp2 + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp3 =
				    tsp2 + ((x + 3 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp4 =
				    tsp2 + ((x + 4 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp5 =
				    tsp2 + ((x + 5 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp6 =
				    tsp2 + ((x + 6 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp7 =
				    tsp2 + ((x + 7 * dx) >> MLIB_SHIFT);

				switch (leftover) {
				case 1:
					s2 = _mm_cvtsi32_si128(sp0[0]);
					s3 = _mm_cvtsi32_si128(sp0[1]);
					break;
				case 2:
					s2 = _mm_cvtsi32_si128(
					    sp0[0] | (sp1[0] << 8));
					s3 = _mm_cvtsi32_si128(
					    sp0[1] | (sp1[1] << 8));
					break;
				case 3:
					s2 = _mm_cvtsi32_si128(
					    sp0[0] | (sp1[0] << 8) |
					    (sp2[0] << 16));
					s3 = _mm_cvtsi32_si128(
					    sp0[1] | (sp1[1] << 8) |
					    (sp2[1] << 16));
					break;
				case 4:
					s2 = _mm_cvtsi32_si128(
					    sp0[0] | (sp1[0] << 8) |
					    (sp2[0] << 16) | (sp3[0] << 24));
					s3 = _mm_cvtsi32_si128(
					    sp0[1] | (sp1[1] << 8) |
					    (sp2[1] << 16) | (sp3[1] << 24));
					break;
				case 5:
					i0 = _mm_cvtsi32_si128(
					    sp0[0] | (sp1[0] << 8) |
					    (sp2[0] << 16) | (sp3[0] << 24));
					i1 = _mm_cvtsi32_si128(sp4[0]);
					s2 = _mm_unpacklo_epi32(i0, i1);
					i2 = _mm_cvtsi32_si128(
					    sp0[1] | (sp1[1] << 8) |
					    (sp2[1] << 16) | (sp3[1] << 24));
					i3 = _mm_cvtsi32_si128(sp4[1]);
					s3 = _mm_unpacklo_epi32(i2, i3);
					break;
				case 6:
					i0 = _mm_cvtsi32_si128(
					    sp0[0] | (sp1[0] << 8) |
					    (sp2[0] << 16) | (sp3[0] << 24));
					i1 = _mm_cvtsi32_si128(
					    sp4[0] | (sp5[0] << 8));
					s2 = _mm_unpacklo_epi32(i0, i1);
					i2 = _mm_cvtsi32_si128(
					    sp0[1] | (sp1[1] << 8) |
					    (sp2[1] << 16) | (sp3[1] << 24));
					i3 = _mm_cvtsi32_si128(
					    sp4[1] | (sp5[1] << 8));
					s3 = _mm_unpacklo_epi32(i2, i3);
					break;
				case 7:
					i0 = _mm_cvtsi32_si128(
					    sp0[0] | (sp1[0] << 8) |
					    (sp2[0] << 16) | (sp3[0] << 24));
					i1 = _mm_cvtsi32_si128(
					    sp4[0] | (sp5[0] << 8) |
					    (sp6[0] << 16));
					s2 = _mm_unpacklo_epi32(i0, i1);
					i2 = _mm_cvtsi32_si128(
					    sp0[1] | (sp1[1] << 8) |
					    (sp2[1] << 16) | (sp3[1] << 24));
					i3 = _mm_cvtsi32_si128(
					    sp4[1] | (sp5[1] << 8) |
					    (sp6[1] << 16));
					s3 = _mm_unpacklo_epi32(i2, i3);
					break;
				default:
					break;
				}

				s2 = _mm_unpacklo_epi8(_mm_setzero_si128(), s2);
				v2 = _mm_mulhi_epu16(s2, rdx);

				s3 = _mm_unpacklo_epi8(_mm_setzero_si128(), s3);
				v3 = _mm_mulhi_epu16(s3, ddx);

				y1 = _mm_add_epi16(v2, v3);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				buffz[i] = dd;
				buff1[i] = y1;
			}

		} else {
			__m128i ddx, rdx;
			mlib_s32 slb = src_stride;

			ddx = ddx_start;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			rdx = _mm_sub_epi16(mask_ffff, ddx);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width / 8; i++) {
				mlib_u8 *sp0 = tsp + (x >> MLIB_SHIFT);
				mlib_u8 *sp1 = tsp + ((x + dx) >> MLIB_SHIFT);
				mlib_u8 *sp2 =
				    tsp + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp3 =
				    tsp + ((x + 3 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp4 =
				    tsp + ((x + 4 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp5 =
				    tsp + ((x + 5 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp6 =
				    tsp + ((x + 6 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp7 =
				    tsp + ((x + 7 * dx) >> MLIB_SHIFT);
				x += 8 * dx;

				__m128i i0, i1, i2, i3, i4, i5, i6, i7;
				__m128i s0, s1, s2, s3;
				__m128i v0, v1, v2, v3, y0, y1, z0, z1, dd;

				i0 = _mm_cvtsi32_si128(
				    sp0[0] | (sp1[0] << 8) |
				    (sp2[0] << 16) | (sp3[0] << 24));
				i1 = _mm_cvtsi32_si128(
				    sp4[0] | (sp5[0] << 8) |
				    (sp6[0] << 16) | (sp7[0] << 24));
				s0 = _mm_unpacklo_epi32(i0, i1);
				s0 = _mm_unpacklo_epi8(_mm_setzero_si128(), s0);
				v0 = _mm_mulhi_epu16(s0, rdx);

				i2 = _mm_cvtsi32_si128(
				    sp0[1] | (sp1[1] << 8) |
				    (sp2[1] << 16) | (sp3[1] << 24));
				i3 = _mm_cvtsi32_si128(
				    sp4[1] | (sp5[1] << 8) |
				    (sp6[1] << 16) | (sp7[1] << 24));
				s1 = _mm_unpacklo_epi32(i2, i3);
				s1 = _mm_unpacklo_epi8(_mm_setzero_si128(), s1);
				v1 = _mm_mulhi_epu16(s1, ddx);
				y0 = _mm_add_epi16(v0, v1);
				z0 = _mm_mulhi_epu16(rdy, y0);

				i4 = _mm_cvtsi32_si128(sp0[slb + 0] |
				    (sp1[slb + 0] << 8) |
				    (sp2[slb + 0] << 16) |
				    (sp3[slb + 0] << 24));
				i5 = _mm_cvtsi32_si128(sp4[slb + 0] |
				    (sp5[slb + 0] << 8) |
				    (sp6[slb + 0] << 16) |
				    (sp7[slb + 0] << 24));
				s2 = _mm_unpacklo_epi32(i4, i5);
				s2 = _mm_unpacklo_epi8(_mm_setzero_si128(), s2);
				v2 = _mm_mulhi_epu16(s2, rdx);

				i6 = _mm_cvtsi32_si128(sp0[slb + 1] |
				    (sp1[slb + 1] << 8) |
				    (sp2[slb + 1] << 16) |
				    (sp3[slb + 1] << 24));
				i7 = _mm_cvtsi32_si128(sp4[slb + 1] |
				    (sp5[slb + 1] << 8) |
				    (sp6[slb + 1] << 16) |
				    (sp7[slb + 1] << 24));
				s3 = _mm_unpacklo_epi32(i6, i7);
				s3 = _mm_unpacklo_epi8(_mm_setzero_si128(), s3);
				v3 = _mm_mulhi_epu16(s3, ddx);
				y1 = _mm_add_epi16(v2, v3);
				z1 = _mm_mulhi_epu16(ddy, y1);

				dd = _mm_add_epi16(z0, z1);

				buffz[i] = dd;
				buff0[i] = y0;
				buff1[i] = y1;
				ddx = _mm_add_epi16(ddx, dx_step);
				rdx = _mm_sub_epi16(mask_ffff, ddx);
			}
			if (width & 7) {
				mlib_s32 leftover = width & 7;
				mlib_u8 *sp0 = tsp + (x >> MLIB_SHIFT);
				mlib_u8 *sp1 = tsp + ((x + dx) >> MLIB_SHIFT);
				mlib_u8 *sp2 =
				    tsp + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp3 =
				    tsp + ((x + 3 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp4 =
				    tsp + ((x + 4 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp5 =
				    tsp + ((x + 5 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp6 =
				    tsp + ((x + 6 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp7 =
				    tsp + ((x + 7 * dx) >> MLIB_SHIFT);

				__m128i i0, i1, i2, i3, i4, i5, i6, i7;
				__m128i s0, s1, s2, s3;
				__m128i v0, v1, v2, v3, y0, y1, z0, z1, dd;

				i0 = _mm_cvtsi32_si128(
				    sp0[0] | (sp1[0] << 8) |
				    (sp2[0] << 16) | (sp3[0] << 24));
				i1 = _mm_cvtsi32_si128(
				    sp4[0] | (sp5[0] << 8) |
				    (sp6[0] << 16) | (sp7[0] << 24));
				s0 = _mm_unpacklo_epi32(i0, i1);
				s0 = _mm_unpacklo_epi8(_mm_setzero_si128(), s0);
				v0 = _mm_mulhi_epu16(s0, rdx);

				i2 = _mm_cvtsi32_si128(
				    sp0[1] | (sp1[1] << 8) |
				    (sp2[1] << 16) | (sp3[1] << 24));
				i3 = _mm_cvtsi32_si128(
				    sp4[1] | (sp5[1] << 8) |
				    (sp6[1] << 16) | (sp7[1] << 24));
				s1 = _mm_unpacklo_epi32(i2, i3);
				s1 = _mm_unpacklo_epi8(_mm_setzero_si128(), s1);
				v1 = _mm_mulhi_epu16(s1, ddx);
				y0 = _mm_add_epi16(v0, v1);
				z0 = _mm_mulhi_epu16(rdy, y0);

				switch (leftover) {
				case 1:
					s2 = _mm_cvtsi32_si128(sp0[slb + 0]);
					s3 = _mm_cvtsi32_si128(sp0[slb + 1]);
					break;
				case 2:
					s2 = _mm_cvtsi32_si128(sp0[slb + 0] |
					    (sp1[slb + 0] << 8));
					s3 = _mm_cvtsi32_si128(sp0[slb + 1] |
					    (sp1[slb + 1] << 8));
					break;
				case 3:
					s2 = _mm_cvtsi32_si128(sp0[slb + 0] |
					    (sp1[slb + 0] << 8) |
					    (sp2[slb + 0] << 16));
					s3 = _mm_cvtsi32_si128(sp0[slb + 1] |
					    (sp1[slb + 1] << 8) |
					    (sp2[slb + 1] << 16));
					break;
				case 4:
					s2 = _mm_cvtsi32_si128(sp0[slb + 0] |
					    (sp1[slb + 0] << 8) |
					    (sp2[slb + 0] << 16) |
					    (sp3[slb + 0] << 24));
					s3 = _mm_cvtsi32_si128(sp0[slb + 1] |
					    (sp1[slb + 1] << 8) |
					    (sp2[slb + 1] << 16) |
					    (sp3[slb + 1] << 24));
					break;
				case 5:
					i4 = _mm_cvtsi32_si128(sp0[slb + 0] |
					    (sp1[slb + 0] << 8) |
					    (sp2[slb + 0] << 16) |
					    (sp3[slb + 0] << 24));
					i5 = _mm_cvtsi32_si128(sp4[slb + 0]);
					s2 = _mm_unpacklo_epi32(i4, i5);
					i6 = _mm_cvtsi32_si128(sp0[slb + 1] |
					    (sp1[slb + 1] << 8) |
					    (sp2[slb + 1] << 16) |
					    (sp3[slb + 1] << 24));
					i7 = _mm_cvtsi32_si128(sp4[slb + 1]);
					s3 = _mm_unpacklo_epi32(i6, i7);
					break;
				case 6:
					i4 = _mm_cvtsi32_si128(sp0[slb + 0] |
					    (sp1[slb + 0] << 8) |
					    (sp2[slb + 0] << 16) |
					    (sp3[slb + 0] << 24));
					i5 = _mm_cvtsi32_si128(sp4[slb + 0] |
					    (sp5[slb + 0] << 8));
					s2 = _mm_unpacklo_epi32(i4, i5);
					i6 = _mm_cvtsi32_si128(sp0[slb + 1] |
					    (sp1[slb + 1] << 8) |
					    (sp2[slb + 1] << 16) |
					    (sp3[slb + 1] << 24));
					i7 = _mm_cvtsi32_si128(sp4[slb + 1] |
					    (sp5[slb + 1] << 8));
					s3 = _mm_unpacklo_epi32(i6, i7);
					break;
				case 7:
					i4 = _mm_cvtsi32_si128(sp0[slb + 0] |
					    (sp1[slb + 0] << 8) |
					    (sp2[slb + 0] << 16) |
					    (sp3[slb + 0] << 24));
					i5 = _mm_cvtsi32_si128(sp4[slb + 0] |
					    (sp5[slb + 0] << 8) |
					    (sp6[slb + 0] << 16));
					s2 = _mm_unpacklo_epi32(i4, i5);
					i6 = _mm_cvtsi32_si128(sp0[slb + 1] |
					    (sp1[slb + 1] << 8) |
					    (sp2[slb + 1] << 16) |
					    (sp3[slb + 1] << 24));
					i7 = _mm_cvtsi32_si128(sp4[slb + 1] |
					    (sp5[slb + 1] << 8) |
					    (sp6[slb + 1] << 16));
					s3 = _mm_unpacklo_epi32(i6, i7);
					break;
				default:
					break;
				}

				s2 = _mm_unpacklo_epi8(_mm_setzero_si128(), s2);
				v2 = _mm_mulhi_epu16(s2, rdx);
				s3 = _mm_unpacklo_epi8(_mm_setzero_si128(), s3);
				v3 = _mm_mulhi_epu16(s3, ddx);
				y1 = _mm_add_epi16(v2, v3);
				z1 = _mm_mulhi_epu16(ddy, y1);

				dd = _mm_add_epi16(z0, z1);

				buffz[i] = dd;
				buff0[i] = y0;
				buff1[i] = y1;
			}
		}

		if (y_step) {

			if (0 == ((mlib_addr)tdp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0, k = 0; i < width / 16; i++) {
					__m128i sd0, sd1, tt0;
					__m128i td0, td1, td2, td3;
					sd0 = buffz[k];
					sd1 = buffz[k + 1];

					td0 = _mm_add_epi16(sd0, k05);
					td1 = _mm_srli_epi16(td0, 8);

					td2 = _mm_add_epi16(sd1, k05);
					td3 = _mm_srli_epi16(td2, 8);

					tt0 = _mm_packus_epi16(td1, td3);

					_mm_store_si128((void *)tdp, tt0);
					tdp += 16;
					k += 2;
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0, k = 0; i < width / 16; i++) {
					__m128i sd0, sd1, tt0;
					__m128i td0, td1, td2, td3;
					sd0 = buffz[k];
					sd1 = buffz[k + 1];

					td0 = _mm_add_epi16(sd0, k05);
					td1 = _mm_srli_epi16(td0, 8);

					td2 = _mm_add_epi16(sd1, k05);
					td3 = _mm_srli_epi16(td2, 8);

					tt0 = _mm_packus_epi16(td1, td3);

					_mm_storeu_si128((void *)tdp, tt0);
					tdp += 16;
					k += 2;
				}
			}

			i *= 2;
			if (i < width / 8) {
				__m128i dd0, dd1, dd2, tt0;
				dd0 = buffz[i];
				dd1 = _mm_add_epi16(dd0, k05);
				dd2 = _mm_srli_epi16(dd1, 8);
				tt0 = _mm_packus_epi16(dd2, dd2);
				_mm_storel_epi64((void *)tdp, tt0);
				tdp += 8;
				i++;
			}

			if (width & 7) {
				__m128i dd0, dd1, dd2, dd3;
				dd0 = buffz[i];
				dd1 = _mm_add_epi16(dd0, k05);
				dd2 = _mm_srli_epi16(dd1, 8);
				dd3 = _mm_packus_epi16(dd2, dd2);
				for (i = 0; i < (width & 7); i++) {
					tdp[i] = ((mlib_u8 *)&dd3)[i];
				}
			}
		}

		dp += dst_stride;
		y += dy;
		ddy = _mm_add_epi16(ddy, dy_step);
		rdy = _mm_sub_epi16(mask_ffff, ddy);
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_ImageZoom_BL_U8_2(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1, *buffz;
	void *buff = buff_arr;
	mlib_u8 *tdp;
	mlib_s32 k, width8, size, y_step, next_step = 2;
	__m128i ddx_start, ddy, rdy, dx_step;
	__m128i mzero, mask_ffff, k05;

	width *= 2;
	width8 = (width + 7) / 8;

	size = (3 * width8 + 1) * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = (__m128i *)((mlib_addr)buff & ~15) + 1;
	buff1 = buff0 + width8;
	buffz = buff1 + width8;

	mzero = _mm_setzero_si128();
	mask_ffff = _mm_set1_epi16(0xFFFF);
	k05 = _mm_set1_epi16(1 << 7);

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 4; i++) {
		((mlib_s16 *)&ddx_start)[2 * i] = x;
		((mlib_s16 *)&ddx_start)[2 * i + 1] = x;
		x += dx;
	}
	dx_step = _mm_set1_epi16(4 * dx);
	ddy = _mm_set1_epi16(y);
	rdy = _mm_sub_epi16(mask_ffff, ddy);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {

			for (i = 0, k = 0; i < width / 16; i++, k += 2) {
				__m128i y0, y1, y2, y3;
				__m128i dd0, dd1, dd2, dd3, dd4;
				y0 = buff0[k];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = buff1[k];
				y1 = _mm_mulhi_epu16(ddy, y1);
				y2 = buff0[k + 1];
				y2 = _mm_mulhi_epu16(rdy, y2);
				y3 = buff1[k + 1];
				y3 = _mm_mulhi_epu16(ddy, y3);
				dd0 = _mm_add_epi16(y0, y1);
				dd1 = _mm_add_epi16(dd0, k05);
				dd2 = _mm_srli_epi16(dd1, 8);

				dd0 = _mm_add_epi16(y2, y3);
				dd1 = _mm_add_epi16(dd0, k05);
				dd3 = _mm_srli_epi16(dd1, 8);

				dd4 = _mm_packus_epi16(dd2, dd3);
				_mm_storeu_si128((void *)tdp, dd4);
				tdp += 16;
			}
			i *= 2;
			if (i < width / 8) {
				__m128i y0, y1;
				__m128i dd0, dd1, dd2, dd3;
				y0 = buff0[i];
				y1 = buff1[i];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = _mm_mulhi_epu16(ddy, y1);
				dd0 = _mm_add_epi16(y0, y1);
				dd1 = _mm_add_epi16(dd0, k05);
				dd2 = _mm_srli_epi16(dd1, 8);
				dd3 = _mm_packus_epi16(dd2, dd2);
				_mm_storel_epi64((void *)tdp, dd3);
				tdp += 8;
				i++;
			}

			if (width & 7) {
				__m128i y0, y1, dd;
				__m128i dd0, dd1, dd2, dd3;
				y0 = buff0[i];
				y1 = buff1[i];
				ZOOM_D();
				for (i = 0; i < (width & 7); i++) {
					tdp[i] = ((mlib_u8 *)&dd3)[i];
				}
			}
		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;

			ddx = ddx_start;
			rdx = _mm_sub_epi16(mask_ffff, ddx);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width / 8; i++) {
				__m128i v2, v3, y0, y1, z0, z1, dd;
				__m128i s2, s3, t2, t3;
				y0 = buff0[i];
				z0 = _mm_mulhi_epu16(rdy, y0);

				mlib_u8 *tsp2 = tsp + src_stride;
				mlib_u8 *sp0 = tsp2 + 2 * (x >> MLIB_SHIFT);
				mlib_u8 *sp1 =
				    tsp2 + 2 * ((x + dx) >> MLIB_SHIFT);
				mlib_u8 *sp2 =
				    tsp2 + 2 * ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp3 =
				    tsp2 + 2 * ((x + 3 * dx) >> MLIB_SHIFT);
				x += 4 * dx;

				LOAD_2s32_to_m128(s2, sp0, sp1);
				LOAD_2s32_to_m128(s3, sp2, sp3);
				t2 = _mm_unpacklo_epi32(s2, s3);
				t3 = _mm_unpackhi_epi32(s2, s3);
				s2 = _mm_unpacklo_epi32(t2, t3);
				s3 = _mm_unpackhi_epi32(t2, t3);

				v2 = _mm_mulhi_epu16(s2, rdx);
				v3 = _mm_mulhi_epu16(s3, ddx);
				ddx = _mm_add_epi16(ddx, dx_step);

				y1 = _mm_add_epi16(v2, v3);
				z1 = _mm_mulhi_epu16(ddy, y1);

				dd = _mm_add_epi16(z0, z1);
				buffz[i] = dd;
				buff1[i] = y1;
				rdx = _mm_sub_epi16(mask_ffff, ddx);
			}
			if (width & 7) {
				mlib_s32 leftover = width & 7;
				__m128i v2, v3, y0, y1, z0, z1, dd;
				__m128i s2, s3, t2, t3;
				y0 = buff0[i];
				z0 = _mm_mulhi_epu16(rdy, y0);

				mlib_u8 *tsp2 = tsp + src_stride;
				mlib_u8 *sp0 = tsp2 + 2 * (x >> MLIB_SHIFT);
				mlib_u8 *sp1 =
				    tsp2 + 2 * ((x + dx) >> MLIB_SHIFT);
				mlib_u8 *sp2 =
				    tsp2 + 2 * ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp3 =
				    tsp2 + 2 * ((x + 3 * dx) >> MLIB_SHIFT);

				/*
				 * Let each letter be a byte, and
				 *
				 * In memory, source buffer:
				 *	+--+--+-+--+--+-+--+--+-+--+--+
				 *	|aA|bB| |cC|dD| |eE|fF| |gG|hH|
				 *	+--+--+-+--+--+-+--+--+-+--+--+
				 *	sp0	sp1	sp2	sp3
				 *
				 * hH are out of bounds if (leftover == 7);
				 * gG are also out of bounds if (leftover == 6);
				 *
				 * In XMM registers:
				 *	+--+--+--+--+--+--+--+--+
				 *	|D0|d0|C0|c0|B0|b0|A0|a0|  s2
				 *	+--+--+--+--+--+--+--+--+
				 *	+--+--+--+--+--+--+--+--+
				 *	|H0|h0|G0|g0|F0|f0|E0|e0|  s3
				 *	+--+--+--+--+--+--+--+--+
				 */
				switch (leftover) {
				case 1:
					s2 = _mm_set1_epi16(*((mlib_s16 *)sp0));
					s2 = _mm_unpacklo_epi8(mzero, s2);
					break;
				case 2:
					LOAD_s32_to_m128(s2, sp0);
					break;
				case 3:
					s2 = _mm_set_epi16(0, 0, 0, 0, 0,
						*((mlib_s16 *)sp1),
						*((mlib_s16 *)sp0 + 1),
						*((mlib_s16 *)sp0));
					s2 = _mm_unpacklo_epi8(mzero, s2);
					break;
				case 4:
					LOAD_2s32_to_m128(s2, sp0, sp1);
					break;
				case 5:
					LOAD_2s32_to_m128(s2, sp0, sp1);
					s3 = _mm_set1_epi16(*((mlib_s16 *)sp2));
					s3 = _mm_unpacklo_epi8(mzero, s3);
					break;
				case 6:
					LOAD_2s32_to_m128(s2, sp0, sp1);
					LOAD_s32_to_m128(s3, sp2);
					break;
				case 7:
					LOAD_2s32_to_m128(s2, sp0, sp1);
					s3 = _mm_set_epi16(0, 0, 0, 0, 0,
						*((mlib_s16 *)sp3),
						*((mlib_s16 *)sp2 + 1),
						*((mlib_s16 *)sp2));
					s3 = _mm_unpacklo_epi8(mzero, s3);
					break;
				default:
					break;
				}

				t2 = _mm_unpacklo_epi32(s2, s3);
				t3 = _mm_unpackhi_epi32(s2, s3);
				s2 = _mm_unpacklo_epi32(t2, t3);
				s3 = _mm_unpackhi_epi32(t2, t3);

				v2 = _mm_mulhi_epu16(s2, rdx);
				v3 = _mm_mulhi_epu16(s3, ddx);

				y1 = _mm_add_epi16(v2, v3);
				z1 = _mm_mulhi_epu16(ddy, y1);

				dd = _mm_add_epi16(z0, z1);
				buffz[i] = dd;
				buff1[i] = y1;
			}

		} else {
			__m128i ddx, rdx;

			ddx = ddx_start;
			rdx = _mm_sub_epi16(mask_ffff, ddx);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			for (i = 0; i < width / 8; i++) {
				__m128i v0, v1, v2, v3, y0, y1, z0, z1, dd;
				__m128i s0, s1, s2, s3;
				__m128i t0, t1, t2, t3;
				mlib_u8 *sp0 = tsp + 2 * (x >> MLIB_SHIFT);
				mlib_u8 *sp1 =
				    tsp + 2 * ((x + dx) >> MLIB_SHIFT);
				mlib_u8 *sp2 =
				    tsp + 2 * ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp3 =
				    tsp + 2 * ((x + 3 * dx) >> MLIB_SHIFT);
				x += 4 * dx;

				LOAD_2s32_to_m128(s0, sp0, sp1);
				LOAD_2s32_to_m128(s1, sp2, sp3);
				t0 = _mm_unpacklo_epi32(s0, s1);
				t1 = _mm_unpackhi_epi32(s0, s1);
				s0 = _mm_unpacklo_epi32(t0, t1);
				s1 = _mm_unpackhi_epi32(t0, t1);

				LOAD_2s32_to_m128(s2, sp0 + src_stride,
				    sp1 + src_stride);
				LOAD_2s32_to_m128(s3, sp2 + src_stride,
				    sp3 + src_stride);
				t2 = _mm_unpacklo_epi32(s2, s3);
				t3 = _mm_unpackhi_epi32(s2, s3);
				s2 = _mm_unpacklo_epi32(t2, t3);
				s3 = _mm_unpackhi_epi32(t2, t3);

				v0 = _mm_mulhi_epu16(s0, rdx);
				v1 = _mm_mulhi_epu16(s1, ddx);
				v2 = _mm_mulhi_epu16(s2, rdx);
				v3 = _mm_mulhi_epu16(s3, ddx);
				ddx = _mm_add_epi16(ddx, dx_step);

				y0 = _mm_add_epi16(v0, v1);
				y1 = _mm_add_epi16(v2, v3);

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				buffz[i] = dd;

				buff0[i] = y0;
				buff1[i] = y1;
				rdx = _mm_sub_epi16(mask_ffff, ddx);
			}
			if (width & 7) {
				mlib_s32 leftover = width & 7;
				mlib_s32 slb = src_stride;
				__m128i v0, v1, v2, v3, y0, y1, z0, z1, dd;
				__m128i s0, s1, s2, s3;
				__m128i t0, t1, t2, t3;
				mlib_u8 *sp0 = tsp + 2 * (x >> MLIB_SHIFT);
				mlib_u8 *sp1 =
				    tsp + 2 * ((x + dx) >> MLIB_SHIFT);
				mlib_u8 *sp2 =
				    tsp + 2 * ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u8 *sp3 =
				    tsp + 2 * ((x + 3 * dx) >> MLIB_SHIFT);

				LOAD_2s32_to_m128(s0, sp0, sp1);
				LOAD_2s32_to_m128(s1, sp2, sp3);
				t0 = _mm_unpacklo_epi32(s0, s1);
				t1 = _mm_unpackhi_epi32(s0, s1);
				s0 = _mm_unpacklo_epi32(t0, t1);
				s1 = _mm_unpackhi_epi32(t0, t1);

				switch (leftover) {
				case 1:
					s2 = _mm_set1_epi16(
					    *((mlib_s16 *)(sp0 + slb)));
					s2 = _mm_unpacklo_epi8(mzero, s2);
					break;
				case 2:
					LOAD_s32_to_m128(s2, sp0 + slb);
					break;
				case 3:
					s2 = _mm_set_epi16(0, 0, 0, 0, 0,
					    *((mlib_s16 *)(sp1 + slb)),
					    *((mlib_s16 *)(sp0 + slb + 2)),
					    *((mlib_s16 *)(sp0 + slb)));
					s2 = _mm_unpacklo_epi8(mzero, s2);
					break;
				case 4:
					LOAD_2s32_to_m128(s2, sp0 + slb,
					    sp1 + slb);
					break;
				case 5:
					LOAD_2s32_to_m128(s2, sp0 + slb,
					    sp1 + slb);
					s3 = _mm_set1_epi16(
					    *((mlib_s16 *)(sp2 + slb)));
					s3 = _mm_unpacklo_epi8(mzero, s3);
					break;
				case 6:
					LOAD_2s32_to_m128(s2, sp0 + slb,
					    sp1 + slb);
					LOAD_s32_to_m128(s3, sp2 + slb);
					break;
				case 7:
					LOAD_2s32_to_m128(s2, sp0 + slb,
					    sp1 + slb);
					s3 = _mm_set_epi16(0, 0, 0, 0, 0,
					    *((mlib_s16 *)(sp3 + slb)),
					    *((mlib_s16 *)(sp2 + slb + 2)),
					    *((mlib_s16 *)(sp2 + slb)));
					s3 = _mm_unpacklo_epi8(mzero, s3);
					break;
				default:
					break;
				}

				t2 = _mm_unpacklo_epi32(s2, s3);
				t3 = _mm_unpackhi_epi32(s2, s3);
				s2 = _mm_unpacklo_epi32(t2, t3);
				s3 = _mm_unpackhi_epi32(t2, t3);

				v0 = _mm_mulhi_epu16(s0, rdx);
				v1 = _mm_mulhi_epu16(s1, ddx);
				v2 = _mm_mulhi_epu16(s2, rdx);
				v3 = _mm_mulhi_epu16(s3, ddx);

				y0 = _mm_add_epi16(v0, v1);
				y1 = _mm_add_epi16(v2, v3);

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				buffz[i] = dd;

				buff0[i] = y0;
				buff1[i] = y1;
			}
		}

		if (y_step) {

			if (0 == ((mlib_addr)tdp & 15)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0, k = 0; i < width / 16; i++) {
					__m128i sd0, sd1, tt0;
					__m128i td0, td1, td2, td3;
					sd0 = buffz[k];
					sd1 = buffz[k + 1];

					td0 = _mm_add_epi16(sd0, k05);
					td1 = _mm_srli_epi16(td0, 8);

					td2 = _mm_add_epi16(sd1, k05);
					td3 = _mm_srli_epi16(td2, 8);

					tt0 = _mm_packus_epi16(td1, td3);

					_mm_store_si128((void *)tdp, tt0);
					tdp += 16;
					k += 2;
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0, k = 0; i < width / 16; i++) {
					__m128i sd0, sd1, tt0;
					__m128i td0, td1, td2, td3;
					sd0 = buffz[k];
					sd1 = buffz[k + 1];

					td0 = _mm_add_epi16(sd0, k05);
					td1 = _mm_srli_epi16(td0, 8);

					td2 = _mm_add_epi16(sd1, k05);
					td3 = _mm_srli_epi16(td2, 8);

					tt0 = _mm_packus_epi16(td1, td3);

					_mm_storeu_si128((void *)tdp, tt0);
					tdp += 16;
					k += 2;
				}
			}

			i *= 2;
			if (i < width / 8) {
				__m128i dd0, dd1, dd2, tt0;
				dd0 = buffz[i];
				dd1 = _mm_add_epi16(dd0, k05);
				dd2 = _mm_srli_epi16(dd1, 8);
				tt0 = _mm_packus_epi16(dd2, dd2);
				_mm_storel_epi64((void *)tdp, tt0);
				tdp += 8;
				i++;
			}

			if (width & 7) {
				__m128i dd0, dd1, dd2, dd3;
				dd0 = buffz[i];
				dd1 = _mm_add_epi16(dd0, k05);
				dd2 = _mm_srli_epi16(dd1, 8);
				dd3 = _mm_packus_epi16(dd2, dd2);
				for (i = 0; i < (width & 7); i++) {
					tdp[i] = ((mlib_u8 *)&dd3)[i];
				}
			}
		}

		dp += dst_stride;
		y += dy;
		ddy = _mm_set1_epi16(y);
		rdy = _mm_sub_epi16(mask_ffff, ddy);
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	INIT_XLINE()                                            \
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;        \
	x0 = x;                                                 \
	yo = (y >> 1) & 0x7FFF;                                 \
	ddy = _mm_set1_epi16(yo);                               \
	rdy = _mm_sub_epi16(mask_8000, ddy);                    \
	tsp0 = tsp;                                             \
	tsp2 = tsp0 + src_stride

/* *********************************************************** */

#define	MUL_ALPHA(res, bmask)                                   \
	aa = _mm_shufflelo_epi16(res, bmask);                   \
	aa = _mm_shufflehi_epi16(aa, bmask);                    \
	aa = _mm_or_si128(aa, amask);                           \
	aa = _mm_mulhi_epu16(aa, scale255);                     \
	res = _mm_mulhi_epu16(res, aa);

/* *********************************************************** */

#define	LOAD_2S32_MUL_ALPHA(res, ptr0, ptr1, bmask)             \
	LOAD_2s32_to_m128(res, ptr0, ptr1);                     \
	MUL_ALPHA(res, bmask)

/* *********************************************************** */

#define	LOAD_S32_MUL_ALPHA(res, ptr0, bmask)                    \
	LOAD_s32_to_m128(res, ptr0);                            \
	MUL_ALPHA(res, bmask)

/* *********************************************************** */

#define	ZOOM_Y()                                                \
	y10 = _mm_mulhi_epu16(rdy, y0);                         \
	z10 = _mm_mulhi_epu16(ddy, y1);                         \
	dd = _mm_add_epi16(y10, z10);                           \
	dd = _mm_slli_epi16(dd, 3);                             \
	buffz[i] = dd;

/* *********************************************************** */

#define	ZOOM_Y1()                                               \
	y10 = _mm_mulhi_epu16(rdy, y0);                         \
	z10 = _mm_mulhi_epu16(ddy, y1);                         \
	dd = _mm_add_epi16(y10, z10);                           \
	dd = _mm_slli_epi16(dd, 2);                             \
	buffz[i] = dd;

/* *********************************************************** */

mlib_status
mlib_s_ImageZoomBlend_BL_U8(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1, *buffx, *buffz,
	    *buffd;
	void *buff = buff_arr;
	mlib_u8 *tdp;
	mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, x2, x3, cx0, cx1, yo;
	mlib_s32 width2, size, y_step, next_step = 2;
	mlib_s32 alp_ind = param->alp_ind;
	__m128i ddx, ddy, rdy, dx64, aa, done, scale255;
	__m128i mzero, mask_7fff, mask_8000, amask, amask256, amaskffff;
	mlib_s32 chan_d = param->chan_d;
	mlib_s32 chan_s = param->channels;

	mzero = _mm_setzero_si128();
	width2 = (width + 1) / 2;

	size = (5 * width2 + 1) * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = (__m128i *)((mlib_addr)buff & ~15) + 1;
	buff1 = buff0 + width2;
	buffx = buff1 + width2;
	buffz = buffx + width2;
	buffd = buffz + width2;

	mask_7fff = _mm_set1_epi16(0x7FFF);
	mask_8000 = _mm_set1_epi16(0x8000);

	x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;
	x1 = x0 + dx;

/* calculate dx for all row */
	dx64 = _mm_set1_epi16(dx);
	ddx =
	    _mm_unpacklo_epi64(_mm_set1_epi16(x0 >> 1),
	    _mm_set1_epi16(x1 >> 1));

	for (i = 0; i < width2; i++) {
		buffx[i] = _mm_and_si128(ddx, mask_7fff);
		ddx = _mm_add_epi16(ddx, dx64);
	}

	if (alp_ind == -1) {
		if (chan_s == 4)
			sp--;
		tsp = sp;
	}

	mzero = _mm_setzero_si128();
	done = _mm_set1_epi16(1 << 15);
	if (alp_ind == -1) {
		amask = _mm_setr_epi32(0xff00, 0, 0xff00, 0);
		amaskffff = _mm_setr_epi32(0xffff, 0, 0xffff, 0);
		amask256 = _mm_setr_epi32(0x0100, 0, 0x0100, 0);
	} else {
		amask = _mm_setr_epi32(0, 0xff000000, 0, 0xff000000);
		amaskffff = _mm_setr_epi32(0, 0xffff0000, 0, 0xffff0000);
		amask256 = _mm_setr_epi32(0, 0x01000000, 0, 0x01000000);
	}

	scale255 = _mm_set1_epi16((mlib_u16)((1 << 15) * 256.0 / 255 + 0.5));

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {
			ddy = _mm_set1_epi16((y >> 1) & 0x7FFF);
			rdy = _mm_sub_epi16(mask_8000, ddy);

			if (chan_s == 4 && alp_ind) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width2; i++) {
					__m128i y0, y1, y10, z10, dd;

					y0 = buff0[i];
					y1 = buff1[i];

					ZOOM_Y();
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width2; i++) {
					__m128i y0, y1, y10, z10, dd;

					y0 = buff0[i];
					y1 = buff1[i];

					ZOOM_Y1();
				}
			}

		} else if (y_step == 1) {
			__m128i *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();

			tsp3 = tsp2 + chan_s;

			if (chan_s == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					__m128i ddx, rdx, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s2, s3;

					cx0 = 3 * (x0 >> MLIB_SHIFT);
					x0 += dx;
					cx1 = 3 * (x0 >> MLIB_SHIFT);
					x0 += dx;

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_2s32_to_m128(s2, tsp2 + cx0,
					    tsp2 + cx1);
					/* -1 and srli for avoiding ABR */
					LOAD_2s32_to_m128(s3, tsp3 + cx0 - 1,
					    tsp3 + cx1 - 1);
					s3 = _mm_srli_si128(s3, 2);

					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = buff0[i];
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y1();

					buff1[i] = y1;
				}
				if (width & 1) {
					__m128i ddx, rdx, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s2, s3;

					cx0 = 3 * (x0 >> MLIB_SHIFT);

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_s32_to_m128(s2, tsp2 + cx0);
					LOAD_s32_to_m128(s3, tsp3 + cx0);

					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = buff0[i];
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y1();

					buff1[i] = y1;
				}
			} else if (!alp_ind) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					__m128i ddx, rdx, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);
					x0 += dx;
					cx1 = (x0 >> MLIB_SHIFT);
					x0 += dx;

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_2s32_to_m128(s2,
					    (mlib_s32 *)tsp2 + cx0,
					    (mlib_s32 *)tsp2 + cx1);
					LOAD_2s32_to_m128(s3,
					    (mlib_s32 *)tsp3 + cx0,
					    (mlib_s32 *)tsp3 + cx1);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = buff0[i];
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y1();

					buff1[i] = y1;
				}
				if (width & 1) {
					__m128i ddx, rdx, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_s32_to_m128(s2,
					    (mlib_s32 *)tsp2 + cx0);
					LOAD_s32_to_m128(s3,
					    (mlib_s32 *)tsp3 + cx0);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = buff0[i];
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y1();

					buff1[i] = y1;
				}
			} else if (alp_ind == -1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					__m128i ddx, rdx, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);
					x0 += dx;
					cx1 = (x0 >> MLIB_SHIFT);
					x0 += dx;

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_2S32_MUL_ALPHA(s2,
					    (mlib_s32 *)tsp2 + cx0,
					    (mlib_s32 *)tsp2 + cx1, 0);
					LOAD_2S32_MUL_ALPHA(s3,
					    (mlib_s32 *)tsp3 + cx0,
					    (mlib_s32 *)tsp3 + cx1, 0);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = buff0[i];
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y();

					buff1[i] = y1;
				}
				if (width & 1) {
					__m128i ddx, rdx, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_S32_MUL_ALPHA(s2,
					    (mlib_s32 *)tsp2 + cx0, 0);
					LOAD_S32_MUL_ALPHA(s3,
					    (mlib_s32 *)tsp3 + cx0, 0);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = buff0[i];
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y();

					buff1[i] = y1;
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					__m128i ddx, rdx, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);
					x0 += dx;
					cx1 = (x0 >> MLIB_SHIFT);
					x0 += dx;

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_2S32_MUL_ALPHA(s2,
					    (mlib_s32 *)tsp2 + cx0,
					    (mlib_s32 *)tsp2 + cx1, 0xff);
					LOAD_2S32_MUL_ALPHA(s3,
					    (mlib_s32 *)tsp3 + cx0,
					    (mlib_s32 *)tsp3 + cx1, 0xff);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = buff0[i];
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y();

					buff1[i] = y1;
				}
				if (width & 1) {
					__m128i ddx, rdx, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_S32_MUL_ALPHA(s2,
					    (mlib_s32 *)tsp2 + cx0, 0xff);
					LOAD_S32_MUL_ALPHA(s3,
					    (mlib_s32 *)tsp3 + cx0, 0xff);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = buff0[i];
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y();

					buff1[i] = y1;
				}
			}

		} else {
			INIT_XLINE();

			tsp1 = tsp0 + chan_s;
			tsp3 = tsp2 + chan_s;

			if (chan_s == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					__m128i ddx, rdx;
					__m128i v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s0, s1, s2, s3;

					cx0 = 3 * (x0 >> MLIB_SHIFT);
					x0 += dx;
					cx1 = 3 * (x0 >> MLIB_SHIFT);
					x0 += dx;

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_2s32_to_m128(s0, tsp0 + cx0,
					    tsp0 + cx1);
					LOAD_2s32_to_m128(s1, tsp1 + cx0,
					    tsp1 + cx1);
					LOAD_2s32_to_m128(s2, tsp2 + cx0,
					    tsp2 + cx1);
					/* -1 and srli for avoiding ABR */
					LOAD_2s32_to_m128(s3, tsp3 + cx0 - 1,
					    tsp3 + cx1 - 1);
					s3 = _mm_srli_si128(s3, 2);

					v0 = _mm_mulhi_epu16(s0, rdx);
					v1 = _mm_mulhi_epu16(s1, ddx);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = _mm_add_epi16(v0, v1);
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y1();

					buff0[i] = y0;
					buff1[i] = y1;
				}
				if (width & 1) {
					__m128i ddx, rdx;
					__m128i v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s0, s1, s2, s3;

					cx0 = 3 * (x0 >> MLIB_SHIFT);

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_s32_to_m128(s0, tsp0 + cx0);
					LOAD_s32_to_m128(s1, tsp1 + cx0);
					LOAD_s32_to_m128(s2, tsp2 + cx0);
					LOAD_s32_to_m128(s3, tsp3 + cx0);

					v0 = _mm_mulhi_epu16(s0, rdx);
					v1 = _mm_mulhi_epu16(s1, ddx);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = _mm_add_epi16(v0, v1);
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y1();

					buff0[i] = y0;
					buff1[i] = y1;
				}
			} else if (!alp_ind) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					__m128i ddx, rdx;
					__m128i v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s0, s1, s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);
					x0 += dx;
					cx1 = (x0 >> MLIB_SHIFT);
					x0 += dx;

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_2s32_to_m128(s0,
					    (mlib_s32 *)tsp0 + cx0,
					    (mlib_s32 *)tsp0 + cx1);
					LOAD_2s32_to_m128(s1,
					    (mlib_s32 *)tsp1 + cx0,
					    (mlib_s32 *)tsp1 + cx1);
					LOAD_2s32_to_m128(s2,
					    (mlib_s32 *)tsp2 + cx0,
					    (mlib_s32 *)tsp2 + cx1);
					LOAD_2s32_to_m128(s3,
					    (mlib_s32 *)tsp3 + cx0,
					    (mlib_s32 *)tsp3 + cx1);
					v0 = _mm_mulhi_epu16(s0, rdx);
					v1 = _mm_mulhi_epu16(s1, ddx);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = _mm_add_epi16(v0, v1);
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y1();

					buff0[i] = y0;
					buff1[i] = y1;
				}
				if (width & 1) {
					__m128i ddx, rdx;
					__m128i v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s0, s1, s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_s32_to_m128(s0,
					    (mlib_s32 *)tsp0 + cx0);
					LOAD_s32_to_m128(s1,
					    (mlib_s32 *)tsp1 + cx0);
					LOAD_s32_to_m128(s2,
					    (mlib_s32 *)tsp2 + cx0);
					LOAD_s32_to_m128(s3,
					    (mlib_s32 *)tsp3 + cx0);
					v0 = _mm_mulhi_epu16(s0, rdx);
					v1 = _mm_mulhi_epu16(s1, ddx);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = _mm_add_epi16(v0, v1);
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y1();

					buff0[i] = y0;
					buff1[i] = y1;
				}
			} else if (alp_ind == -1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					__m128i ddx, rdx;
					__m128i v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s0, s1, s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);
					x0 += dx;
					cx1 = (x0 >> MLIB_SHIFT);
					x0 += dx;

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_2S32_MUL_ALPHA(s0,
					    (mlib_s32 *)tsp0 + cx0,
					    (mlib_s32 *)tsp0 + cx1, 0);
					LOAD_2S32_MUL_ALPHA(s1,
					    (mlib_s32 *)tsp1 + cx0,
					    (mlib_s32 *)tsp1 + cx1, 0);
					LOAD_2S32_MUL_ALPHA(s2,
					    (mlib_s32 *)tsp2 + cx0,
					    (mlib_s32 *)tsp2 + cx1, 0);
					LOAD_2S32_MUL_ALPHA(s3,
					    (mlib_s32 *)tsp3 + cx0,
					    (mlib_s32 *)tsp3 + cx1, 0);
					v0 = _mm_mulhi_epu16(s0, rdx);
					v1 = _mm_mulhi_epu16(s1, ddx);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = _mm_add_epi16(v0, v1);
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y();

					buff0[i] = y0;
					buff1[i] = y1;
				}
				if (width & 1) {
					__m128i ddx, rdx;
					__m128i v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s0, s1, s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_S32_MUL_ALPHA(s0,
					    (mlib_s32 *)tsp0 + cx0, 0);
					LOAD_S32_MUL_ALPHA(s1,
					    (mlib_s32 *)tsp1 + cx0, 0);
					LOAD_S32_MUL_ALPHA(s2,
					    (mlib_s32 *)tsp2 + cx0, 0);
					LOAD_S32_MUL_ALPHA(s3,
					    (mlib_s32 *)tsp3 + cx0, 0);
					v0 = _mm_mulhi_epu16(s0, rdx);
					v1 = _mm_mulhi_epu16(s1, ddx);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = _mm_add_epi16(v0, v1);
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y();

					buff0[i] = y0;
					buff1[i] = y1;
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					__m128i ddx, rdx;
					__m128i v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s0, s1, s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);
					x0 += dx;
					cx1 = (x0 >> MLIB_SHIFT);
					x0 += dx;

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_2S32_MUL_ALPHA(s0,
					    (mlib_s32 *)tsp0 + cx0,
					    (mlib_s32 *)tsp0 + cx1, 0xff);
					LOAD_2S32_MUL_ALPHA(s1,
					    (mlib_s32 *)tsp1 + cx0,
					    (mlib_s32 *)tsp1 + cx1, 0xff);
					LOAD_2S32_MUL_ALPHA(s2,
					    (mlib_s32 *)tsp2 + cx0,
					    (mlib_s32 *)tsp2 + cx1, 0xff);
					LOAD_2S32_MUL_ALPHA(s3,
					    (mlib_s32 *)tsp3 + cx0,
					    (mlib_s32 *)tsp3 + cx1, 0xff);
					v0 = _mm_mulhi_epu16(s0, rdx);
					v1 = _mm_mulhi_epu16(s1, ddx);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = _mm_add_epi16(v0, v1);
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y();

					buff0[i] = y0;
					buff1[i] = y1;
				}
				if (width & 1) {
					__m128i ddx, rdx;
					__m128i v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					__m128i s0, s1, s2, s3;

					cx0 = (x0 >> MLIB_SHIFT);

					ddx = buffx[i];
					rdx = _mm_sub_epi16(mask_8000, ddx);

					LOAD_S32_MUL_ALPHA(s0,
					    (mlib_s32 *)tsp0 + cx0, 0xff);
					LOAD_S32_MUL_ALPHA(s1,
					    (mlib_s32 *)tsp1 + cx0, 0xff);
					LOAD_S32_MUL_ALPHA(s2,
					    (mlib_s32 *)tsp2 + cx0, 0xff);
					LOAD_S32_MUL_ALPHA(s3,
					    (mlib_s32 *)tsp3 + cx0, 0xff);
					v0 = _mm_mulhi_epu16(s0, rdx);
					v1 = _mm_mulhi_epu16(s1, ddx);
					v2 = _mm_mulhi_epu16(s2, rdx);
					v3 = _mm_mulhi_epu16(s3, ddx);

					y0 = _mm_add_epi16(v0, v1);
					y1 = _mm_add_epi16(v2, v3);

					ZOOM_Y();

					buff0[i] = y0;
					buff1[i] = y1;
				}
			}
		}

		mlib_s_ImageBlendLine(param, tdp, buffz, buffd);

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
