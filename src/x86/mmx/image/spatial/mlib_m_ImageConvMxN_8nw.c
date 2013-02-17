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

#pragma ident	"@(#)mlib_m_ImageConvMxN_8nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal function for mlib_ImageConvMxN
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

#define	DTYPE	mlib_u8

/* *********************************************************** */
static mlib_s32 mlib_round_8[16] = {
	0x00400040, 0x00200020, 0x00100010, 0x00080008,
	0x00040004, 0x00020002, 0x00010001, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000
};

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
		ssize,                                              \
		xsize,                                              \
		dsize,                                              \
		esize,                                              \
		emask,                                              \
		buff_ind = 0;                                       \
	    __m64 *                                                 \
	    pbuff, *                                                \
		dp,                                                 \
		drnd;                                               \
	    mlib_s32                                                \
		i,                                                  \
		j,                                                  \
		l

/* *********************************************************** */
#define	MULADD(z, x, y)	z = _mm_add_pi16(z, _mm_mulhi_pi16(x, y))

/* *********************************************************** */
#undef  KSIZE
#define	MAX_N	11

mlib_status
mlib_m_convMxNnw_u8(
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
	__m64 *buff0, *buff1, *buff2, *buff3, *buffn, *buffd;
	__m64 karr_local[MAX_N * MAX_N], *karr = karr_local;
	__m64 dd, d0, d1;
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

	ssize = nchan * wid;
	dsize = (ssize + 7) / 8;
	esize = 2 * dsize + 4;
	pbuff = mlib_malloc((n + 1) * esize * sizeof (__m64));

	if (pbuff == NULL) {
		if (buffs != buffs_local)
			mlib_free(buffs);
		if (karr != karr_local)
			mlib_free(buffs);
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

/* load kernel */
	for (i = 0; i < m * n; i++) {
		mlib_u32 iker = ((mlib_u32)kern[i]) >> 16;
		__m64 dker = _m_from_int(iker | (iker << 16));

		karr[i] = _mm_unpacklo_pi32(dker, dker);
	}

/* init buffer */
/* mmx_to_double_dup(mlib_round_8[31 - scale]); */
	drnd = _m_zero;
	for (i = 0; i < (xsize + 7) / 8; i++) {
		buffd[2 * i] = drnd;
		buffd[2 * i + 1] = drnd;
	}

/* load first lines */
	for (l = 0; l < n - 1; l++) {
		buffn = buffs[l];
		for (i = 0; i < dsize; i++) {
			__m64 s0 = ((__m64 *) sl)[i];

			buffn[2 * i] =
			    _mm_slli_pi16(_mm_unpacklo_pi8(s0, _m_zero), 7);
			buffn[2 * i + 1] =
			    _mm_slli_pi16(_mm_unpackhi_pi8(s0, _m_zero), 7);
		}

		sl += sll;
	}

	scale -= 25;

	for (j = 0; j < hgt; j++) {
		__m64 *pk = karr, k0, k1, k2, k3;

		buff = buffs + buff_ind;
/* load next line */
		buffn = buff[n - 1];
		for (i = 0; i < dsize; i++) {
			__m64 s0 = ((__m64 *) sl)[i];

			buffn[2 * i] =
			    _mm_slli_pi16(_mm_unpacklo_pi8(s0, _m_zero), 7);
			buffn[2 * i + 1] =
			    _mm_slli_pi16(_mm_unpackhi_pi8(s0, _m_zero), 7);
		}

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
						d0 = buffd[2 * i];
						d1 = buffd[2 * i + 1];

						MULADD(d0, buff0[2 * i], k0);
						MULADD(d1, buff0[2 * i + 1],
						    k0);

						buffd[2 * i] = d0;
						buffd[2 * i + 1] = d1;
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
						d0 = buffd[2 * i];
						d1 = buffd[2 * i + 1];

						MULADD(d0, buff0[2 * i], k0);
						MULADD(d1, buff0[2 * i + 1],
						    k0);
						MULADD(d0, buff1[2 * i], k1);
						MULADD(d1, buff1[2 * i + 1],
						    k1);

						buffd[2 * i] = d0;
						buffd[2 * i + 1] = d1;
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
						d0 = buffd[2 * i];
						d1 = buffd[2 * i + 1];

						MULADD(d0, buff0[2 * i], k0);
						MULADD(d1, buff0[2 * i + 1],
						    k0);
						MULADD(d0, buff1[2 * i], k1);
						MULADD(d1, buff1[2 * i + 1],
						    k1);
						MULADD(d0, buff2[2 * i], k2);
						MULADD(d1, buff2[2 * i + 1],
						    k2);

						buffd[2 * i] = d0;
						buffd[2 * i + 1] = d1;
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
						d0 = buffd[2 * i];
						d1 = buffd[2 * i + 1];

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

						buffd[2 * i] = d0;
						buffd[2 * i + 1] = d1;
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
			for (i = 0; i < xsize / 8; i++) {
				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];

				MULADD(d0, buff0[2 * i], k0);
				MULADD(d1, buff0[2 * i + 1], k0);

				dd = _mm_packs_pu16(_mm_srai_pi16(d0, scale),
				    _mm_srai_pi16(d1, scale));
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];

				MULADD(d0, buff0[2 * i], k0);
				MULADD(d1, buff0[2 * i + 1], k0);

				dd = _mm_packs_pu16(_mm_srai_pi16(d0, scale),
				    _mm_srai_pi16(d1, scale));
				mmx_pst_8(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

		} else if (jk_size == 2) {
			for (i = 0; i < xsize / 8; i++) {
				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];

				MULADD(d0, buff0[2 * i], k0);
				MULADD(d1, buff0[2 * i + 1], k0);
				MULADD(d0, buff1[2 * i], k1);
				MULADD(d1, buff1[2 * i + 1], k1);

				dd = _mm_packs_pu16(_mm_srai_pi16(d0, scale),
				    _mm_srai_pi16(d1, scale));
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];

				MULADD(d0, buff0[2 * i], k0);
				MULADD(d1, buff0[2 * i + 1], k0);
				MULADD(d0, buff1[2 * i], k1);
				MULADD(d1, buff1[2 * i + 1], k1);

				dd = _mm_packs_pu16(_mm_srai_pi16(d0, scale),
				    _mm_srai_pi16(d1, scale));
				mmx_pst_8(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

		} else if (jk_size == 3) {
			for (i = 0; i < xsize / 8; i++) {
				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];

				MULADD(d0, buff0[2 * i], k0);
				MULADD(d1, buff0[2 * i + 1], k0);
				MULADD(d0, buff1[2 * i], k1);
				MULADD(d1, buff1[2 * i + 1], k1);
				MULADD(d0, buff2[2 * i], k2);
				MULADD(d1, buff2[2 * i + 1], k2);

				dd = _mm_packs_pu16(_mm_srai_pi16(d0, scale),
				    _mm_srai_pi16(d1, scale));
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];

				MULADD(d0, buff0[2 * i], k0);
				MULADD(d1, buff0[2 * i + 1], k0);
				MULADD(d0, buff1[2 * i], k1);
				MULADD(d1, buff1[2 * i + 1], k1);
				MULADD(d0, buff2[2 * i], k2);
				MULADD(d1, buff2[2 * i + 1], k2);

				dd = _mm_packs_pu16(_mm_srai_pi16(d0, scale),
				    _mm_srai_pi16(d1, scale));
				mmx_pst_8(dd, dp + i, emask);

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

		} else {	/* if (jk_size == 4) */

			for (i = 0; i < xsize / 8; i++) {
				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];

				MULADD(d0, buff0[2 * i], k0);
				MULADD(d1, buff0[2 * i + 1], k0);
				MULADD(d0, buff1[2 * i], k1);
				MULADD(d1, buff1[2 * i + 1], k1);
				MULADD(d0, buff2[2 * i], k2);
				MULADD(d1, buff2[2 * i + 1], k2);
				MULADD(d0, buff3[2 * i], k3);
				MULADD(d1, buff3[2 * i + 1], k3);

				dd = _mm_packs_pu16(_mm_srai_pi16(d0, scale),
				    _mm_srai_pi16(d1, scale));
				dp[i] = dd;

				buffd[2 * i] = drnd;
				buffd[2 * i + 1] = drnd;
			}

			if (emask) {
				d0 = buffd[2 * i];
				d1 = buffd[2 * i + 1];

				MULADD(d0, buff0[2 * i], k0);
				MULADD(d1, buff0[2 * i + 1], k0);
				MULADD(d0, buff1[2 * i], k1);
				MULADD(d1, buff1[2 * i + 1], k1);
				MULADD(d0, buff2[2 * i], k2);
				MULADD(d1, buff2[2 * i + 1], k2);
				MULADD(d0, buff3[2 * i], k3);
				MULADD(d1, buff3[2 * i + 1], k3);

				dd = _mm_packs_pu16(_mm_srai_pi16(d0, scale),
				    _mm_srai_pi16(d1, scale));
				mmx_pst_8(dd, dp + i, emask);

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

	mlib_free(pbuff);

	if (buffs != buffs_local)
		mlib_free(buffs);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
