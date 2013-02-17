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

#pragma ident	"@(#)mlib_m_ImageConvMxN_u16nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal function for mlib_ImageConvMxN
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

#define	TYPE_U16

/* *********************************************************** */
#ifdef TYPE_U16
#define	FUNC(NAME)	mlib_m_##NAME##_u16
#define	XOR_8000(x)	_mm_xor_si64(x, mask8000)
#else
#define	FUNC(NAME)	mlib_m_##NAME##_s16
#define	XOR_8000(x)	(x)
#endif

/* *********************************************************** */
#define	DTYPE	mlib_s16

/* *********************************************************** */
#define	DEF_VARS()                                                  \
	DTYPE *sl, *sp, *dl;                                        \
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
		xsize,                                              \
		emask,                                              \
		shift,                                              \
		buff_ind = 0;                                       \
	    __m64 *                                                 \
		dp,                                                 \
		drnd;                                               \
	    mlib_s32                                                \
		i,                                                  \
		j,                                                  \
		l

/* *********************************************************** */
#define	MULADD(z, x, y)                                         \
	xx = XOR_8000(x);                                       \
	ah = _mm_mullo_pi16(xx, y);                             \
	al = _mm_mulhi_pi16(xx, y);                             \
	mh = _mm_unpacklo_pi16(ah, al);                         \
	ml = _mm_unpackhi_pi16(ah, al);                         \
	z##h = _mm_add_pi32(z##h, mh);                          \
	z##l = _mm_add_pi32(z##l, ml)

/* *********************************************************** */
#ifdef TYPE_U16

#define	PACK_RES(dd, d0, d1)                                    \
	d0 = _mm_add_pi32(_mm_srai_pi32(d0, shift), ker_off);   \
	d1 = _mm_add_pi32(_mm_srai_pi32(d1, shift), ker_off);   \
	dd = XOR_8000(_mm_packs_pi32(d0, d1))

#else

#define	PACK_RES(dd, d0, d1)                                    \
	d0 = _mm_srai_pi32(d0, shift);                          \
	d1 = _mm_srai_pi32(d1, shift);                          \
	dd = _mm_packs_pi32(d0, d1)

#endif

/* *********************************************************** */
#undef  KSIZE
#define	MAX_N	11

mlib_status FUNC(
    convMxNnw) (
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 *kern,
    mlib_s32 scale)
{
	__m64 *buffs_local[2 * MAX_N], **buffs = buffs_local, **buff;
	__m64 *buff0, *buff1, *buff2, *buff3, *buffd;
	__m64 karr_local[MAX_N * MAX_N], *karr = karr_local;
	__m64 al, ah, ml, mh, xx;
	__m64 dd, d0h, d0l, d1h, d1l;

#ifdef TYPE_U16
	__m64 ker_off, mask8000;
	mlib_s32 ker_sum = 0;
#endif
	mlib_s32 ik, jk, jk_size, coff, ik_last = m - 1;

	DEF_VARS();

	if (n > MAX_N) {
		buffs = mlib_malloc(2 * n * sizeof (__m64 *));

		if (buffs == NULL)
			return (MLIB_FAILURE);
	}

	if (m * n > MAX_N * MAX_N) {
		karr = mlib_malloc(m * n * sizeof (__m64));

		if (karr == NULL) {
			if (buffs != buffs_local)
				mlib_free(buffs);
			return (MLIB_FAILURE);
		}
	}

	sl = adr_src;
	dl = adr_dst + dn * dll + dm * nchan;

	wid -= (m - 1);
	hgt -= (n - 1);
	xsize = nchan * wid;
	emask = (0xF0 >> (xsize & 3)) & 0xF;

	buffd = mlib_malloc(4 * ((xsize + 7) / 8) * sizeof (__m64));

	if (buffd == NULL) {
		if (buffs != buffs_local)
			mlib_free(buffs);
		if (karr != karr_local)
			mlib_free(buffs);
		return (MLIB_FAILURE);
	}

/* load kernel */
	shift = scale - 16;
	for (i = 0; i < m * n; i++) {
		mlib_s32 iker = kern[i] >> 16;
		__m64 dker = _m_from_int((iker & 0xFFFF) | (iker << 16));

		karr[i] = _mm_unpacklo_pi32(dker, dker);
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

	ker_off = _m_from_int(ker_sum - 0x8000);
	ker_off = _mm_unpacklo_pi32(ker_off, ker_off);
	mask8000 = _m_from_int(0x80008000);
	mask8000 = _mm_unpacklo_pi32(mask8000, mask8000);
#endif

/* init buffer */
	drnd = _m_zero;
	for (i = 0; i < (xsize + 7) / 8; i++) {
		buffd[4 * i] = drnd;
		buffd[4 * i + 1] = drnd;
		buffd[4 * i + 2] = drnd;
		buffd[4 * i + 3] = drnd;
	}

/* save first lines */
	for (l = 0; l < n - 1; l++) {
		buffs[l] = (__m64 *) sl;
		sl += sll;
	}

	for (j = 0; j < hgt; j++) {
		__m64 *pk = karr, k0, k1, k2, k3;

		buff = buffs + buff_ind;
		jk = buff_ind + (n - 1);
		buffs[jk] = (__m64 *) sl;

		if (jk >= n)
			jk -= n;
		else
			jk += n;
		buffs[jk] = (__m64 *) sl;

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
					    (__m64 *) ((mlib_s16 *)buff[jk] +
					    coff);

					for (i = 0; i < (xsize + 7) / 8; i++) {
						d0h = buffd[4 * i];
						d0l = buffd[4 * i + 1];
						d1h = buffd[4 * i + 2];
						d1l = buffd[4 * i + 3];

						MULADD(d0, buff0[2 * i], k0);
						MULADD(d1, buff0[2 * i + 1],
						    k0);

						buffd[4 * i] = d0h;
						buffd[4 * i + 1] = d0l;
						buffd[4 * i + 2] = d1h;
						buffd[4 * i + 3] = d1l;
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
					    (__m64 *) ((mlib_s16 *)buff[jk] +
					    coff);
					buff1 =
					    (__m64 *) ((mlib_s16 *)buff[jk +
					    1] + coff);

					for (i = 0; i < (xsize + 7) / 8; i++) {
						d0h = buffd[4 * i];
						d0l = buffd[4 * i + 1];
						d1h = buffd[4 * i + 2];
						d1l = buffd[4 * i + 3];

						MULADD(d0, buff0[2 * i], k0);
						MULADD(d1, buff0[2 * i + 1],
						    k0);
						MULADD(d0, buff1[2 * i], k1);
						MULADD(d1, buff1[2 * i + 1],
						    k1);

						buffd[4 * i] = d0h;
						buffd[4 * i + 1] = d0l;
						buffd[4 * i + 2] = d1h;
						buffd[4 * i + 3] = d1l;
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
					    (__m64 *) ((mlib_s16 *)buff[jk] +
					    coff);
					buff1 =
					    (__m64 *) ((mlib_s16 *)buff[jk +
					    1] + coff);
					buff2 =
					    (__m64 *) ((mlib_s16 *)buff[jk +
					    2] + coff);

					for (i = 0; i < (xsize + 7) / 8; i++) {
						d0h = buffd[4 * i];
						d0l = buffd[4 * i + 1];
						d1h = buffd[4 * i + 2];
						d1l = buffd[4 * i + 3];

						MULADD(d0, buff0[2 * i], k0);
						MULADD(d1, buff0[2 * i + 1],
						    k0);
						MULADD(d0, buff1[2 * i], k1);
						MULADD(d1, buff1[2 * i + 1],
						    k1);
						MULADD(d0, buff2[2 * i], k2);
						MULADD(d1, buff2[2 * i + 1],
						    k2);

						buffd[4 * i] = d0h;
						buffd[4 * i + 1] = d0l;
						buffd[4 * i + 2] = d1h;
						buffd[4 * i + 3] = d1l;
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
					    (__m64 *) ((mlib_s16 *)buff[jk] +
					    coff);
					buff1 =
					    (__m64 *) ((mlib_s16 *)buff[jk +
					    1] + coff);
					buff2 =
					    (__m64 *) ((mlib_s16 *)buff[jk +
					    2] + coff);
					buff3 =
					    (__m64 *) ((mlib_s16 *)buff[jk +
					    3] + coff);

					for (i = 0; i < (xsize + 7) / 8; i++) {
						d0h = buffd[4 * i];
						d0l = buffd[4 * i + 1];
						d1h = buffd[4 * i + 2];
						d1l = buffd[4 * i + 3];

						MULADD(d0, buff0[2 * i], k0);
						MULADD(d1, buff0[2 * i + 1],
						    k0);
						MULADD(d0, buff1[2 * i], k1);
						MULADD(d1, buff1[2 * i + 1],
						    k1);
						MULADD(d0, buff2[2 * i], k2);
						MULADD(d1, buff2[2 * i + 1],
						    k2);
						MULADD(d0, buff3[2 * i], k3);
						MULADD(d1, buff3[2 * i + 1],
						    k3);

						buffd[4 * i] = d0h;
						buffd[4 * i + 1] = d0l;
						buffd[4 * i + 2] = d1h;
						buffd[4 * i + 3] = d1l;
					}
				}

				pk += 4 * m;
			}
		}

/*
 * ****************************************
 * *          Final iteration            **
 * ****************************************
 */

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
		buff0 = (__m64 *) ((mlib_s16 *)buff[0] + coff);
		buff1 = (__m64 *) ((mlib_s16 *)buff[1] + coff);
		buff2 = (__m64 *) ((mlib_s16 *)buff[2] + coff);
		buff3 = (__m64 *) ((mlib_s16 *)buff[3] + coff);

		dp = (__m64 *) dl;

		if (jk_size == 1) {
			for (i = 0; i < xsize / 4; i++) {
				d0h = buffd[2 * i];
				d0l = buffd[2 * i + 1];

				MULADD(d0, buff0[i], k0);

				PACK_RES(dd, d0h, d0l);
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				d0h = buffd[2 * i];
				d0l = buffd[2 * i + 1];

				MULADD(d0, buff0[i], k0);

				PACK_RES(dd, d0h, d0l);
				mmx_pst_16(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

		} else if (jk_size == 2) {
			for (i = 0; i < xsize / 4; i++) {
				d0h = buffd[2 * i];
				d0l = buffd[2 * i + 1];

				MULADD(d0, buff0[i], k0);
				MULADD(d0, buff1[i], k1);

				PACK_RES(dd, d0h, d0l);
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				d0h = buffd[2 * i];
				d0l = buffd[2 * i + 1];

				MULADD(d0, buff0[i], k0);
				MULADD(d0, buff1[i], k1);

				PACK_RES(dd, d0h, d0l);
				mmx_pst_16(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

		} else if (jk_size == 3) {
			for (i = 0; i < xsize / 4; i++) {
				d0h = buffd[2 * i];
				d0l = buffd[2 * i + 1];

				MULADD(d0, buff0[i], k0);
				MULADD(d0, buff1[i], k1);
				MULADD(d0, buff2[i], k2);

				PACK_RES(dd, d0h, d0l);
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				d0h = buffd[2 * i];
				d0l = buffd[2 * i + 1];

				MULADD(d0, buff0[i], k0);
				MULADD(d0, buff1[i], k1);
				MULADD(d0, buff2[i], k2);

				PACK_RES(dd, d0h, d0l);
				mmx_pst_16(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

		} else {	/* if (jk_size == 4) */

			for (i = 0; i < xsize / 4; i++) {
				d0h = buffd[2 * i];
				d0l = buffd[2 * i + 1];

				MULADD(d0, buff0[i], k0);
				MULADD(d0, buff1[i], k1);
				MULADD(d0, buff2[i], k2);
				MULADD(d0, buff3[i], k3);

				PACK_RES(dd, d0h, d0l);
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				d0h = buffd[2 * i];
				d0l = buffd[2 * i + 1];

				MULADD(d0, buff0[i], k0);
				MULADD(d0, buff1[i], k1);
				MULADD(d0, buff2[i], k2);
				MULADD(d0, buff3[i], k3);

				PACK_RES(dd, d0h, d0l);
				mmx_pst_16(dd, dp + i, emask);

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

	_mm_empty();

	mlib_free(buffd);

	if (buffs != buffs_local)
		mlib_free(buffs);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
