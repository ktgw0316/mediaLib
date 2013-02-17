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

#pragma ident	"@(#)mlib_s_ImageConvMxN_u16nw.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal function for mlib_ImageConvMxN
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_sse_utils.h>

#define	TYPE_U16

/* *********************************************************** */
#ifdef TYPE_U16
#define	FUNC(NAME)	mlib_s_##NAME##_u16
#define	XOR_8000(x)	_mm_xor_si128(x, mask8000)
#else
#define	FUNC(NAME)	mlib_s_##NAME##_s16
#define	XOR_8000(x)	(x)
#endif

/* *********************************************************** */
#define	DTYPE	mlib_s16

/* *********************************************************** */
#define	DEF_VARS()                                                  \
	DTYPE *sl, *sp, *dl, *dp;                                   \
	mlib_s32 hgt = mlib_ImageGetHeight(src);                    \
	mlib_s32 wid = mlib_ImageGetWidth(src);                     \
	mlib_s32 nchan = mlib_ImageGetChannels(dst);                \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (DTYPE);   \
	    mlib_s32                                                \
		dll = mlib_ImageGetStride(dst) / sizeof (DTYPE);    \
	    DTYPE *                                                 \
		adr_src = (DTYPE *) mlib_ImageGetData(src);         \
	    DTYPE *                                                 \
		adr_dst = (DTYPE *) mlib_ImageGetData(dst);         \
	    mlib_s32                                                \
		ssize,                                              \
		xsize,                                              \
		esize,                                              \
		emask,                                              \
		buff_ind = 0;                                       \
	    __m128i *                                               \
		pbuff,                                              \
		drnd;                                               \
	    mlib_s32                                                \
		i,                                                  \
		j,                                                  \
		l,                                                  \
		shift

/* *********************************************************** */
#define	MULADD(z, x, y)                                         \
	xx = XOR_8000(x);                                       \
	al = _mm_mullo_epi16(xx, y);                            \
	ah = _mm_mulhi_epi16(xx, y);                            \
	ml = _mm_unpacklo_epi16(al, ah);                        \
	mh = _mm_unpackhi_epi16(al, ah);                        \
	z##l = _mm_add_epi32(z##l, ml);                         \
	z##h = _mm_add_epi32(z##h, mh)

/* *********************************************************** */
#ifdef TYPE_U16

#define	PACK_RES(dd, d0, d1)                                    \
	d0 = _mm_srai_epi32(d0, shift);                         \
	d1 = _mm_srai_epi32(d1, shift);                         \
	d0 = _mm_add_epi32(d0, ker_off);                        \
	d1 = _mm_add_epi32(d1, ker_off);                        \
	dd = XOR_8000(_mm_packs_epi32(d0, d1))

#else

#define	PACK_RES(dd, d0, d1)                                    \
	d0 = _mm_srai_epi32(d0, shift);                         \
	d1 = _mm_srai_epi32(d1, shift);                         \
	dd = _mm_packs_epi32(d0, d1)

#endif

/* *********************************************************** */

#undef  KSIZE
#define	MAX_N	11

mlib_status
FUNC(convMxNnw)(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 *kern,
    mlib_s32 scale)
{
	__m128i *buffs_local[2 * MAX_N], **buffs = buffs_local, **buff;
	__m128i *buff0, *buff1, *buff2, *buff3, *buffn, *buffd;
	__m128i karr_local[MAX_N * MAX_N], *karr = karr_local;
	__m128i dd, d0, d0l, d0h, mask;
	__m128i al, ah, ml, mh, xx;

#ifdef TYPE_U16
	__m128i ker_off, mask8000;
	mlib_s32 ker_sum = 0;
#endif
	__m128i mzero = _mm_setzero_si128();
	mlib_s32 ik, jk, jk_size, coff, ik_last = m - 1;

	DEF_VARS();

	if (n > MAX_N) {
		buffs = __mlib_malloc(2 * n * sizeof (__m128i *));

		if (buffs == NULL)
			return (MLIB_FAILURE);
	}

	if (m * n > MAX_N * MAX_N) {
		karr = __mlib_malloc(m * n * sizeof (__m128i));

		if (karr == NULL) {
			if (buffs != buffs_local)
				__mlib_free(buffs);
			return (MLIB_FAILURE);
		}
	}

	sl = adr_src;
	dl = adr_dst + dn * dll + dm * nchan;

	ssize = nchan * wid;
	esize = (ssize + 3) / 4 + 4;
	pbuff = __mlib_malloc((n + 1) * esize * sizeof (__m128i));

	if (pbuff == NULL) {
		if (buffs != buffs_local)
			__mlib_free(buffs);
		if (karr != karr_local)
			__mlib_free(buffs);
		return (MLIB_FAILURE);
	}

	for (i = 0; i < n; i++)
		buffs[i] = pbuff + i * esize;
	for (i = 0; i < n; i++)
		buffs[n + i] = buffs[i];
	buffd = buffs[n - 1] + esize;

	wid -= (m - 1);
	hgt -= (n - 1);
	xsize = ssize - nchan * (m - 1);
	emask = (0xFF00 >> (xsize & 7)) & 0xFF;
	mask = LOAD_2x64(mask_pst_16, (emask >> 4) & 0xf, emask & 0xf);

/* load kernel */
	shift = scale - 16;
	for (i = 0; i < m * n; i++) {
		mlib_s32 iker = kern[i] >> 16;

		karr[i] = _mm_set1_epi16(iker);
#ifdef TYPE_U16
		ker_sum += iker;
#endif
	}

#ifdef TYPE_U16
	if (shift <= 15) {
		ker_sum <<= (15 - shift);
	} else {
		ker_sum >>= (shift - 15);
	}

	ker_off = _mm_set1_epi32(ker_sum - 0x8000);
	mask8000 = _mm_set1_epi16(0x8000);
#endif

/* init buffer */
/* mmx_to_double_dup(mlib_round_8[31 - scale]); */
	drnd = mzero;
	for (i = 0; i < (xsize + 3) / 4; i++) {
		buffd[i] = drnd;
	}

/* load first lines */
	for (l = 0; l < n - 1; l++) {
		buffs[l] = (void *)sl;
		sl += sll;
	}

	scale -= 25;

	for (j = 0; j < hgt; j++) {
		__m128i *pk = karr, k0, k1, k2, k3;

		buff = buffs + buff_ind;

		jk = buff_ind + (n - 1);
		buffs[jk] = (void *)sl;
		if (jk >= n)
			jk -= n;
		else
			jk += n;
		buffs[jk] = (void *)sl;

		for (jk = 0; jk < n; jk += jk_size) {
			jk_size = n - jk;

			if (jk_size >= 6)
				jk_size = 4;
			if (jk_size == 5)
				jk_size = 3;
			coff = 0;

			if (jk_size == 1) {

				for (ik = 0; ik < m; ik++, coff += nchan) {
					if (!jk && ik == ik_last)
						continue;

					k0 = pk[ik];

					buff0 =
					    (__m128i *) ((mlib_s16 *)buff[jk] +
					    coff);

					for (i = 0; i < (xsize + 7) / 8; i++) {
						d0l = buffd[2 * i];
						d0h = buffd[2 * i + 1];
						MULADD(d0,
						    _mm_loadu_si128(buff0 + i),
						    k0);
						buffd[2 * i] = d0l;
						buffd[2 * i + 1] = d0h;
					}
				}

				pk += m;

			} else if (jk_size == 2) {

				for (ik = 0; ik < m; ik++, coff += nchan) {
					if (!jk && ik == ik_last)
						continue;

					k0 = pk[ik];
					k1 = pk[ik + m];

					buff0 =
					    (__m128i *) ((mlib_s16 *)buff[jk] +
					    coff);
					buff1 =
					    (__m128i *) ((mlib_s16 *)buff[jk +
					    1] + coff);

					for (i = 0; i < (xsize + 7) / 8; i++) {
						d0l = buffd[2 * i];
						d0h = buffd[2 * i + 1];
						MULADD(d0,
						    _mm_loadu_si128(buff0 + i),
						    k0);
						MULADD(d0,
						    _mm_loadu_si128(buff1 + i),
						    k1);
						buffd[2 * i] = d0l;
						buffd[2 * i + 1] = d0h;
					}
				}

				pk += 2 * m;

			} else if (jk_size == 3) {

				for (ik = 0; ik < m; ik++, coff += nchan) {
					if (!jk && ik == ik_last)
						continue;

					k0 = pk[ik];
					k1 = pk[ik + m];
					k2 = pk[ik + 2 * m];

					buff0 =
					    (__m128i *) ((mlib_s16 *)buff[jk] +
					    coff);
					buff1 =
					    (__m128i *) ((mlib_s16 *)buff[jk +
					    1] + coff);
					buff2 =
					    (__m128i *) ((mlib_s16 *)buff[jk +
					    2] + coff);

					for (i = 0; i < (xsize + 7) / 8; i++) {
						d0l = buffd[2 * i];
						d0h = buffd[2 * i + 1];
						MULADD(d0,
						    _mm_loadu_si128(buff0 + i),
						    k0);
						MULADD(d0,
						    _mm_loadu_si128(buff1 + i),
						    k1);
						MULADD(d0,
						    _mm_loadu_si128(buff2 + i),
						    k2);
						buffd[2 * i] = d0l;
						buffd[2 * i + 1] = d0h;
					}
				}

				pk += 3 * m;

			} else {
/* jk_size == 4 */

				for (ik = 0; ik < m; ik++, coff += nchan) {
					if (!jk && ik == ik_last)
						continue;

					k0 = pk[ik];
					k1 = pk[ik + m];
					k2 = pk[ik + 2 * m];
					k3 = pk[ik + 3 * m];

					buff0 =
					    (__m128i *) ((mlib_s16 *)buff[jk] +
					    coff);
					buff1 =
					    (__m128i *) ((mlib_s16 *)buff[jk +
					    1] + coff);
					buff2 =
					    (__m128i *) ((mlib_s16 *)buff[jk +
					    2] + coff);
					buff3 =
					    (__m128i *) ((mlib_s16 *)buff[jk +
					    3] + coff);

					for (i = 0; i < (xsize + 7) / 8; i++) {
						d0l = buffd[2 * i];
						d0h = buffd[2 * i + 1];
						MULADD(d0,
						    _mm_loadu_si128(buff0 + i),
						    k0);
						MULADD(d0,
						    _mm_loadu_si128(buff1 + i),
						    k1);
						MULADD(d0,
						    _mm_loadu_si128(buff2 + i),
						    k2);
						MULADD(d0,
						    _mm_loadu_si128(buff3 + i),
						    k3);
						buffd[2 * i] = d0l;
						buffd[2 * i + 1] = d0h;
					}
				}

				pk += 4 * m;
			}
		}

		/* ******** Final iteration ******** */

		jk_size = n;

		if (jk_size >= 6)
			jk_size = 4;
		if (jk_size == 5)
			jk_size = 3;

		k0 = karr[ik_last];
		k1 = karr[ik_last + m];
		k2 = karr[ik_last + 2 * m];
		k3 = karr[ik_last + 3 * m];

		coff = ik_last * nchan;
		buff0 = (__m128i *) ((mlib_s16 *)buff[0] + coff);
		buff1 = (__m128i *) ((mlib_s16 *)buff[1] + coff);
		buff2 = (__m128i *) ((mlib_s16 *)buff[2] + coff);
		buff3 = (__m128i *) ((mlib_s16 *)buff[3] + coff);

		dp = dl;

		if (jk_size == 1) {
			for (i = 0; i < xsize / 8; i++) {
				d0l = buffd[2 * i];
				d0h = buffd[2 * i + 1];
				MULADD(d0, _mm_loadu_si128(buff0 + i), k0);
				PACK_RES(d0, d0l, d0h);

				_mm_storeu_si128((void *)dp, d0);
				dp += 8;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (xsize & 7) {
				d0l = buffd[2 * i];
				d0h = buffd[2 * i + 1];
				MULADD(d0, _mm_loadu_si128(buff0 + i), k0);
				PACK_RES(d0, d0l, d0h);

				dd = _mm_loadu_si128((void *)dp);
				dd = _mm_or_si128(_mm_and_si128(mask, d0),
				    _mm_andnot_si128(mask, dd));
				_mm_storeu_si128((void *)dp, dd);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

		} else if (jk_size == 2) {
			for (i = 0; i < xsize / 8; i++) {
				d0l = buffd[2 * i];
				d0h = buffd[2 * i + 1];
				MULADD(d0, _mm_loadu_si128(buff0 + i), k0);
				MULADD(d0, _mm_loadu_si128(buff1 + i), k1);
				PACK_RES(d0, d0l, d0h);

				_mm_storeu_si128((void *)dp, d0);
				dp += 8;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (xsize & 7) {
				d0l = buffd[2 * i];
				d0h = buffd[2 * i + 1];
				MULADD(d0, _mm_loadu_si128(buff0 + i), k0);
				MULADD(d0, _mm_loadu_si128(buff1 + i), k1);
				PACK_RES(d0, d0l, d0h);

				dd = _mm_loadu_si128((void *)dp);
				dd = _mm_or_si128(_mm_and_si128(mask, d0),
				    _mm_andnot_si128(mask, dd));
				_mm_storeu_si128((void *)dp, dd);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

		} else if (jk_size == 3) {
			for (i = 0; i < xsize / 8; i++) {
				d0l = buffd[2 * i];
				d0h = buffd[2 * i + 1];
				MULADD(d0, _mm_loadu_si128(buff0 + i), k0);
				MULADD(d0, _mm_loadu_si128(buff1 + i), k1);
				MULADD(d0, _mm_loadu_si128(buff2 + i), k2);
				PACK_RES(d0, d0l, d0h);

				_mm_storeu_si128((void *)dp, d0);
				dp += 8;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (xsize & 7) {
				d0l = buffd[2 * i];
				d0h = buffd[2 * i + 1];
				MULADD(d0, _mm_loadu_si128(buff0 + i), k0);
				MULADD(d0, _mm_loadu_si128(buff1 + i), k1);
				MULADD(d0, _mm_loadu_si128(buff2 + i), k2);
				PACK_RES(d0, d0l, d0h);

				dd = _mm_loadu_si128((void *)dp);
				dd = _mm_or_si128(_mm_and_si128(mask, d0),
				    _mm_andnot_si128(mask, dd));
				_mm_storeu_si128((void *)dp, dd);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

		} else {	/* if (jk_size == 4) */

			for (i = 0; i < xsize / 8; i++) {
				d0l = buffd[2 * i];
				d0h = buffd[2 * i + 1];
				MULADD(d0, _mm_loadu_si128(buff0 + i), k0);
				MULADD(d0, _mm_loadu_si128(buff1 + i), k1);
				MULADD(d0, _mm_loadu_si128(buff2 + i), k2);
				MULADD(d0, _mm_loadu_si128(buff3 + i), k3);
				PACK_RES(d0, d0l, d0h);

				_mm_storeu_si128((void *)dp, d0);
				dp += 8;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (xsize & 7) {
				d0l = buffd[2 * i];
				d0h = buffd[2 * i + 1];
				MULADD(d0, _mm_loadu_si128(buff0 + i), k0);
				MULADD(d0, _mm_loadu_si128(buff1 + i), k1);
				MULADD(d0, _mm_loadu_si128(buff2 + i), k2);
				MULADD(d0, _mm_loadu_si128(buff3 + i), k3);
				PACK_RES(d0, d0l, d0h);

				dd = _mm_loadu_si128((void *)dp);
				dd = _mm_or_si128(_mm_and_si128(mask, d0),
				    _mm_andnot_si128(mask, dd));
				_mm_storeu_si128((void *)dp, dd);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}
		}

		sl += sll;
		dl += dll;

		buff_ind++;

		if (buff_ind >= n)
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	if (buffs != buffs_local)
		__mlib_free(buffs);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
