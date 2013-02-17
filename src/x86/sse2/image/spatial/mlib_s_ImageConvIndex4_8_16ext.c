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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident   "@(#)mlib_s_ImageConvIndex4_8_16ext.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageConv*Index on U8 type
 *      and MLIB_EDGE_SRC_EXTEND mask
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_s_ImageConvIndex.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/*
 *  This defines switches between functions in
 *  files:
 *         mlib_s_ImageConvIndex3_8_8ext.c,
 *         mlib_s_ImageConvIndex4_8_8ext.c,
 *         mlib_s_ImageConvIndex3_8_16ext.c,
 *         mlib_s_ImageConvIndex4_8_16ext.c
 */

/* *********************************************************** */
#define	DTYPE	mlib_s16
#define	LTYPE	mlib_u8

/* *********************************************************** */

#define	CONV_FUNC(KERN)                                                    \
	mlib_conv##KERN##_Index4_8_16ext(mlib_image *dst,                  \
	    const mlib_image *src, const mlib_s32 *kern, mlib_s32 scale,   \
	    const void *colormap)

/* *********************************************************** */

#define	CONV_FUNC_MxN                                                     \
	mlib_convMxN_Index4_8_16ext(mlib_image *dst,                      \
	    const mlib_image *src, mlib_s32 m, mlib_s32 n,                \
	    mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t, mlib_s32 dy_b,   \
	    const mlib_s32 *kern, mlib_s32 scale,                         \
	    const void *colormap)

#define	ColorTrue2IndexLine	mlib_ImageColorTrue2IndexLine_U8_S16_4

/* *********************************************************** */

#define	NCHAN	4

/* *********************************************************** */

#define	DEF_VARS                                                      \
	DTYPE    *sl, *sp, *dl;                                       \
	mlib_s32 hgt = mlib_ImageGetHeight(src);                      \
	mlib_s32 wid = mlib_ImageGetWidth(src);                       \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (DTYPE);     \
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (DTYPE);     \
	DTYPE    *adr_src = (DTYPE *)mlib_ImageGetData(src);          \
	DTYPE    *adr_dst = (DTYPE *)mlib_ImageGetData(dst);          \
	mlib_s32 ssize, xsize, dsize, esize, buff_ind = 0;            \
	__m128i *pbuff;                                               \
	mlib_s32 shift = scale - 25;                                  \
	__m128i drnd = _mm_set1_epi16(                                \
		(shift > 0) ? (1 << (shift - 1)) : 0);                    \
	mlib_s32 i, j, l

/* *********************************************************** */
#define	DEF_EXTRA_VARS                                                   \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);              \
	LTYPE **lut_table = (LTYPE **) mlib_ImageGetLutData(colormap);   \
	LTYPE *ltbl0 = lut_table[0] - offset;                            \
	LTYPE *ltbl1 = lut_table[1] - offset;                            \
	LTYPE *ltbl2 = lut_table[2] - offset;                            \
	LTYPE *ltbl3 = lut_table[3] - offset

/* *********************************************************** */
#define	GET_KERN()                                             \
	ker1 = _mm_set1_epi16((kern[0] >> 16) & 0xFFFF);           \
	ker2 = _mm_set1_epi16((kern[1] >> 16) & 0xFFFF);           \
	ker3 = _mm_set1_epi16((kern[2] >> 16) & 0xFFFF);           \
	ker4 = _mm_set1_epi16((kern[3] >> 16) & 0xFFFF);           \

/* *********************************************************** */
#define	LOAD_SRC_NF()                                          \
	{                                                          \
	    mlib_s32 s0 = sp[0], s1 = sp[1];                       \
	    __m128i t0;                                            \
	                                                           \
		t0 = _mm_setr_epi16(                                   \
				ltbl0[s0], ltbl1[s0], ltbl2[s0], ltbl3[s0],    \
				ltbl0[s1], ltbl1[s1], ltbl2[s1], ltbl3[s1]);   \
		t0 = _mm_slli_epi16(t0, 7);                            \
		_mm_storeu_si128(buffn + i, t0);                       \
	    sp += 2;                                               \
	}

/* *********************************************************** */

#define	EXT_LEFT(buff, dx_l, i)                                      \
	* ((buff) + 0 + NCHAN * i) = *((buff) + 0 + NCHAN * dx_l);   \
	*((buff) + 1 + NCHAN * i) = *((buff) + 1 + NCHAN * dx_l);    \
	*((buff) + 2 + NCHAN * i) = *((buff) + 2 + NCHAN * dx_l);    \
	*((buff) + 3 + NCHAN * i) = *((buff) + 3 + NCHAN * dx_l)

/* *********************************************************** */

#define	EXT_RIGHT(buff, dx_l, i)                                \
	* ((buff) + 0 + NCHAN * i + xsize + dx_l * NCHAN) =     \
	    *((buff) + 0 + NCHAN * (dx_l - 1) + xsize);         \
	*((buff) + 1 + NCHAN * i + xsize + dx_l * NCHAN) =      \
	    *((buff) + 1 + NCHAN * (dx_l - 1) + xsize);         \
	*((buff) + 2 + NCHAN * i + xsize + dx_l * NCHAN) =      \
	    *((buff) + 2 + NCHAN * (dx_l - 1) + xsize);         \
	*((buff) + 3 + NCHAN * i + xsize + dx_l * NCHAN) =      \
	    *((buff) + 3 + NCHAN * (dx_l - 1) + xsize)

/* *********************************************************** */

#define	KSIZE	2

mlib_status
CONV_FUNC(2x2)
{
	__m128i *buffs[2 * (KSIZE + 1)];
	__m128i *buff0, *buff1, *buffn, *buffd, *buffe;
	__m128i s0, s1;
	__m128i d0, d1;
	__m128i ker1, ker2, ker3, ker4;

	DEF_VARS;
	DEF_EXTRA_VARS;
	GET_KERN();

	sl = adr_src;
	dl = adr_dst;

	ssize = NCHAN * (wid + (KSIZE - 1));
	dsize = (NCHAN * wid + 7) / 8;
	esize = (ssize + 7) / 8 + 4;
	pbuff = __mlib_malloc(2 * (KSIZE + 4) * esize * sizeof (__m128i));

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++)
		buffs[i] = pbuff + i * esize * 2;
	for (i = 0; i < (KSIZE + 1); i++)
		buffs[(KSIZE + 1) + i] = buffs[i];
	buffd = buffs[KSIZE] + esize * 2;
	buffe = buffd + 2 * esize;

	xsize = NCHAN * wid;

	for (l = 0; l < KSIZE; l++) {
		__m128i *buffn = buffs[l];

		sp = sl;

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < dsize; i++) {
			LOAD_SRC_NF();
		}

		EXT_RIGHT((mlib_s16 *)buffn, 0, 0);

		if (l < hgt - 1)
			sl += sll;
	}

	for (j = 0; j < hgt; j++) {
		__m128i **buffc = buffs + buff_ind;
		__m128i *tbuf0, *tbuf1;

		sp = sl;

		buff0 = buffc[0];
		buff1 = buffc[1];
		buffn = buffc[KSIZE];

		tbuf0 = (__m128i *)(((mlib_u8 *)buff0) + NCHAN * 2);
		tbuf1 = (__m128i *)(((mlib_u8 *)buff1) + NCHAN * 2);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < (xsize + 7) / 8; i++) {
			s0 = _mm_loadu_si128(tbuf0 + i);
			s1 = _mm_loadu_si128(tbuf1 + i);

			d0 = _mm_mulhi_epi16(s0, ker2);
			d1 = _mm_mulhi_epi16(s1, ker4);
			d0 = _mm_add_epi16(d0, d1);

			_mm_storeu_si128(buffd + i, d0);
		}
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < dsize; i++) {
			__m128i d00, d10, d0;
			__m128i s00 = _mm_loadu_si128(buff0 + i);
			__m128i s10 = _mm_loadu_si128(buff1 + i);

			d00 = _mm_mulhi_epi16(s00, ker1);
			d10 = _mm_mulhi_epi16(s10, ker3);
			d00 = _mm_add_epi16(d00, d10);

			d0 = _mm_loadu_si128(buffd + i);
			d0 = _mm_add_epi16(d0, d00);

			d0 = _mm_add_epi16(d0, drnd);

			d0 = _mm_srai_epi16(d0, shift);
			d0 = _mm_packus_epi16(d0, d0);

			_mm_storel_epi64((void *)
				(((__m64 *)buffe) + i), d0);

			LOAD_SRC_NF();
		}

		EXT_RIGHT((mlib_s16 *)buffn, 0, 0);

		ColorTrue2IndexLine((void *)buffe, dl, wid, colormap);

		if (j < hgt - 3)
			sl += sll;
		dl += dll;

		buff_ind++;

		if (buff_ind >= (KSIZE + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	3

mlib_status
CONV_FUNC(3x3)
{
	__m128i *buffs[2 * (KSIZE + 1)];
	__m128i *buff0, *buff1, *buff2, *buffn, *buffd, *buffe, *tbuf;
	__m128i s0, s1, s2;
	__m128i d0, d1, d00, d10, d20;
	__m128i k0, k1, k2;
	mlib_s32 ik, ik_last, off;

	DEF_VARS;
	DEF_EXTRA_VARS;

	sl = adr_src;
	dl = adr_dst;

	ssize = NCHAN * (wid + (KSIZE - 1));
	dsize = (NCHAN * wid + 7) / 8;
	esize = (ssize + 7) / 8 + 4;
	pbuff = __mlib_malloc(2 * (KSIZE + 4) * esize * sizeof (__m128i));

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++)
		buffs[i] = pbuff + i * esize * 2;
	for (i = 0; i < (KSIZE + 1); i++)
		buffs[(KSIZE + 1) + i] = buffs[i];
	buffd = buffs[KSIZE] + esize * 2;
	buffe = buffd + 2 * esize;

	xsize = NCHAN * wid;

	for (l = 0; l < KSIZE; l++) {
		__m128i *buffn = buffs[l];

		sp = sl;
		tbuf = buffn;
		buffn = (__m128i *)(((mlib_s16 *)buffn) + NCHAN);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < dsize; i++) {
			LOAD_SRC_NF();
		}

		buffn = tbuf;

		EXT_LEFT((mlib_s16 *)buffn, 1, 0);
		EXT_RIGHT((mlib_s16 *)buffn, 1, 0);

		if ((l >= 1) && (l < hgt))
			sl += sll;
	}

/* init buffer */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < (xsize + 7) / 8; i++) {
		buffd[2 * i] = drnd;
		buffd[2 * i + 1] = drnd;
	}

	for (j = 0; j < hgt; j++) {
		__m128i **buffc = buffs + buff_ind;
		__m128i *pbuff0, *pbuff1, *pbuff2;

		sp = sl;

		pbuff0 = buffc[0];
		pbuff1 = buffc[1];
		pbuff2 = buffc[2];
		buffn = buffc[KSIZE];
		tbuf = buffn;
		buffn = (__m128i *)(((mlib_s16 *)buffn) + NCHAN);

		ik_last = 0;

		for (ik = 0; ik < KSIZE; ik++) {
			k0 = _mm_set1_epi16(
				(kern[ik] >> 16) & 0xFFFF);
			k1 = _mm_set1_epi16(
				(kern[ik + KSIZE] >> 16) & 0xFFFF);
			k2 = _mm_set1_epi16(
				(kern[ik + 2 * KSIZE] >> 16) & 0xFFFF);

			off = ik * NCHAN * 2;
			buff0 = (__m128i *)(((mlib_u8 *)pbuff0) + off);
			buff1 = (__m128i *)(((mlib_u8 *)pbuff1) + off);
			buff2 = (__m128i *)(((mlib_u8 *)pbuff2) + off);

			if (ik == ik_last)
				continue;

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < (xsize + 7) / 8; i++) {
				s0 = _mm_loadu_si128(buff0 + i);
				s1 = _mm_loadu_si128(buff1 + i);
				s2 = _mm_loadu_si128(buff2 + i);

				d00 = _mm_mulhi_epi16(s0, k0);
				d10 = _mm_mulhi_epi16(s1, k1);
				d20 = _mm_mulhi_epi16(s2, k2);

				d0 = _mm_loadu_si128(buffd + i);

				d0 = _mm_add_epi16(d00, d0);
				d0 = _mm_add_epi16(d10, d0);
				d0 = _mm_add_epi16(d20, d0);

				_mm_storeu_si128(buffd + i, d0);
			}
		}

		k0 = _mm_set1_epi16(
			(kern[ik_last] >> 16) & 0xFFFF);
		k1 = _mm_set1_epi16(
			(kern[ik_last + KSIZE] >> 16) & 0xFFFF);
		k2 = _mm_set1_epi16(
			(kern[ik_last + 2 * KSIZE] >> 16) & 0xFFFF);

		off = ik_last * NCHAN * 2;
		buff0 = (__m128i *)(((mlib_u8 *)pbuff0) + off);
		buff1 = (__m128i *)(((mlib_u8 *)pbuff1) + off);
		buff2 = (__m128i *)(((mlib_u8 *)pbuff2) + off);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < dsize; i++) {
			__m128i d00, d10, d20, d0;

			__m128i s00 = _mm_loadu_si128(buff0 + i);
			__m128i s10 = _mm_loadu_si128(buff1 + i);
			__m128i s20 = _mm_loadu_si128(buff2 + i);

			d00 = _mm_mulhi_epi16(s00, k0);
			d10 = _mm_mulhi_epi16(s10, k1);
			d20 = _mm_mulhi_epi16(s20, k2);

			d0 = _mm_loadu_si128(buffd + i);
			d0 = _mm_add_epi16(d0, d00);
			d0 = _mm_add_epi16(d0, d10);
			d0 = _mm_add_epi16(d0, d20);

			d0 = _mm_srai_epi16(d0, shift);
			d0 = _mm_packus_epi16(d0, d0);

			_mm_storel_epi64((void *)
				(((__m64 *)buffe) + i), d0);

			_mm_storeu_si128(buffd + i, drnd);
			LOAD_SRC_NF();

		}

		buffn = tbuf;

		EXT_LEFT((mlib_s16 *)buffn, 1, 0);
		EXT_RIGHT((mlib_s16 *)buffn, 1, 0);

		ColorTrue2IndexLine((void *)buffe, dl, wid, colormap);

		if (j < hgt - 3)
			sl += sll;
		dl += dll;

		buff_ind++;

		if (buff_ind >= (KSIZE + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	MAX_N	11

mlib_status CONV_FUNC_MxN
{
	__m128i *buffs_local[3 * (MAX_N + 1)], **buffs = buffs_local, **buff;
	__m128i *buff0, *buff1, *buff2, *buff3, *buffn, *buffd, *buffe, *tbuf;
	__m128i s0, s1, s2;
	__m128i d00, d10, d20;
	__m128i d0, d1;
	__m128i k0, k1, k2;
	mlib_s32 ik, jk, ik_last, jk_size, coff, off, gap;
	DEF_VARS;
	DEF_EXTRA_VARS;

	if (n > MAX_N) {
		buffs = __mlib_malloc(3 * (n + 1) * sizeof (__m128i *));

		if (buffs == NULL)
			return (MLIB_FAILURE);
	}
	buff = buffs + 2 * (n + 1);

	sl = adr_src;
	dl = adr_dst;

	ssize = NCHAN * (wid + (m - 1));
	dsize = (NCHAN * (wid + m - 1 - dx_l - dx_r) + 7) / 8;
	esize = (ssize + 7) / 8 + 4;
	pbuff = __mlib_malloc(2 * (n + 4) * esize * sizeof (__m128i));

	if (pbuff == NULL) {
		if (buffs != buffs_local)
			__mlib_free(buffs);
		return (MLIB_FAILURE);
	}

	for (i = 0; i < (n + 1); i++)
		buffs[i] = pbuff + i * esize * 2;
	for (i = 0; i < (n + 1); i++)
		buffs[(n + 1) + i] = buffs[i];
	buffd = buffs[n] + esize * 2;
	buffe = buffd + 2 * esize;

	gap = NCHAN * dx_l;
	xsize = NCHAN * wid;

	for (l = 0; l < n; l++) {
		__m128i *buffn = buffs[l];

		sp = sl;
		tbuf = buffn;
		buffn = (__m128i *)(((mlib_s16 *)buffn) + gap);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < dsize; i++) {
			LOAD_SRC_NF();
		}

		buffn = tbuf;

		for (i = 0; i < dx_l; i++) {
			EXT_LEFT((mlib_s16 *)buffn, dx_l, i);
		}

		for (i = 0; i < dx_r; i++) {
			EXT_RIGHT((mlib_s16 *)buffn, (m - 1 - dx_r), i);
		}

		if ((l >= dy_t) && (l < hgt + n - dy_b - 2))
			sl += sll;
	}

/* init buffer */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < dsize; i++) {
		_mm_storeu_si128(buffd + i, drnd);
	}

	for (j = 0; j < hgt; j++) {
		__m128i **buffc = buffs + buff_ind;
		mlib_s32 *pk = (mlib_s32 *)kern;

		sp = sl;

		for (l = 0; l < n; l++) {
			buff[l] = buffc[l];
		}

		buffn = buffc[n];
		tbuf = buffn;
		buffn = (__m128i *)(((mlib_s16 *)buffn) + gap);

		ik_last = 0;

		for (jk = 0; jk < n; jk += jk_size) {
			jk_size = n - jk;

			if (jk_size >= 5)
				jk_size = 3;

			if (jk_size == 4)
				jk_size = 2;

			coff = 0;

			if (jk_size == 2) {

				for (ik = 0; ik < m; ik++, coff += NCHAN) {
					if (!jk && ik == ik_last)
						continue;

					k0 = _mm_set1_epi16(
						(pk[ik] >> 16) & 0xFFFF);
					k1 = _mm_set1_epi16(
						(pk[ik + m] >> 16) & 0xFFFF);

					buff0 = (__m128i *)(((mlib_u8 *)
						(buff[jk])) + coff * 2);
					buff1 = (__m128i *)(((mlib_u8 *)
						(buff[jk + 1])) + coff * 2);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
					for (i = 0; i < dsize; i++) {
						s0 = _mm_loadu_si128(buff0 + i);
						s1 = _mm_loadu_si128(buff1 + i);

						d00 = _mm_mulhi_epi16(s0, k0);
						d10 = _mm_mulhi_epi16(s1, k1);

						d0 = _mm_loadu_si128(buffd + i);
						d0 = _mm_add_epi16(d00, d0);
						d0 = _mm_add_epi16(d10, d0);
						_mm_storeu_si128(buffd + i, d0);
					}
				}

				pk += 2 * m;
			} else {
/* if (jk_size == 3) */

				for (ik = 0; ik < m; ik++, coff += NCHAN) {
					if (!jk && ik == ik_last)
						continue;

					k0 = _mm_set1_epi16(
						(pk[ik] >> 16) & 0xFFFF);
					k1 = _mm_set1_epi16(
						(pk[ik + m] >> 16) & 0xFFFF);
					k2 = _mm_set1_epi16(
						(pk[ik + 2 * m] >>
						16) & 0xFFFF);

					buff0 = (__m128i *)(((mlib_u8 *)
						(buff[jk])) + coff * 2);
					buff1 = (__m128i *)(((mlib_u8 *)
						(buff[jk + 1])) + coff * 2);
					buff2 = (__m128i *)(((mlib_u8 *)
						(buff[jk + 2])) + coff * 2);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
					for (i = 0; i < dsize; i++) {
						s0 = _mm_loadu_si128(buff0 + i);
						s1 = _mm_loadu_si128(buff1 + i);
						s2 = _mm_loadu_si128(buff2 + i);

						d00 = _mm_mulhi_epi16(s0, k0);
						d10 = _mm_mulhi_epi16(s1, k1);
						d20 = _mm_mulhi_epi16(s2, k2);

						d0 = _mm_loadu_si128(buffd + i);
						d0 = _mm_add_epi16(d00, d0);
						d0 = _mm_add_epi16(d10, d0);
						d0 = _mm_add_epi16(d20, d0);

						_mm_storeu_si128(buffd + i, d0);
					}
				}

				pk += 3 * m;
			}
		}

/* ******************* Final iteration ****************** */

		jk_size = n;

		if (jk_size >= 5)
			jk_size = 3;

		if (jk_size == 4)
			jk_size = 2;

		k0 = _mm_set1_epi16(
			(kern[ik_last] >> 16) & 0xFFFF);
		k1 = _mm_set1_epi16(
			(kern[ik_last + m] >> 16) & 0xFFFF);
		k2 = _mm_set1_epi16(
			(kern[ik_last + 2 * m] >> 16) & 0xFFFF);

		off = ik_last * NCHAN;

		buff0 = (__m128i *)(((mlib_u8 *)
			(buff[0])) + off * 2);
		buff1 = (__m128i *)(((mlib_u8 *)
			(buff[1])) + off * 2);
		buff2 = (__m128i *)(((mlib_u8 *)
			(buff[2])) + off * 2);

		if (jk_size == 2) {

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < dsize; i++) {
				__m128i d00, d10, d0;
				__m128i s00 = _mm_loadu_si128(buff0 + i);
				__m128i s10 = _mm_loadu_si128(buff1 + i);

				d00 = _mm_mulhi_epi16(s00, k0);
				d10 = _mm_mulhi_epi16(s10, k1);

				d0 = _mm_loadu_si128(buffd + i);
				d0 = _mm_add_epi16(d0, d00);
				d0 = _mm_add_epi16(d0, d10);
				d0 = _mm_srai_epi16(d0, shift);
				d0 = _mm_packus_epi16(d0, d0);

				_mm_storel_epi64((void *)
					(((__m64 *)buffe) + i), d0);
				_mm_storeu_si128(buffd + i, drnd);

				LOAD_SRC_NF();
			}
		} else {
/* if (jk_size == 3) */

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < dsize; i++) {
				__m128i d00, d10, d20, d0;
				__m128i s00 = _mm_loadu_si128(buff0 + i);
				__m128i s10 = _mm_loadu_si128(buff1 + i);
				__m128i s20 = _mm_loadu_si128(buff2 + i);

				d00 = _mm_mulhi_epi16(s00, k0);
				d10 = _mm_mulhi_epi16(s10, k1);
				d20 = _mm_mulhi_epi16(s20, k2);

				d0 = _mm_loadu_si128(buffd + i);
				d0 = _mm_add_epi16(d0, d00);
				d0 = _mm_add_epi16(d0, d10);
				d0 = _mm_add_epi16(d0, d20);
				d0 = _mm_srai_epi16(d0, shift);
				d0 = _mm_packus_epi16(d0, d0);

				_mm_storel_epi64((void *)
					(((__m64 *)buffe) + i), d0);
				_mm_storeu_si128(buffd + i, drnd);

				LOAD_SRC_NF();

			}
		}

		buffn = tbuf;

		for (i = 0; i < dx_l; i++) {
			EXT_LEFT((mlib_s16 *)buffn, dx_l, i);
		}

		for (i = 0; i < dx_r; i++) {
			EXT_RIGHT((mlib_s16 *)buffn, (m - 1 - dx_r), i);
		}

		ColorTrue2IndexLine((void *)buffe, dl, wid, colormap);

		if (j < hgt - dy_b - 2)
			sl += sll;
		dl += dll;

		buff_ind++;

		if (buff_ind >= (n + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	if (buffs != buffs_local)
		__mlib_free(buffs);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
