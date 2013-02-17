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

#pragma ident	"@(#)mlib_m_ImageChannelExtract_f.c	9.4	07/11/05 SMI"

/* *********************************************************** */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif


#include <mlib_mmx_utils.h>
#include <mlib_image.h>

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_42L(sd0, sd1, dd)                    \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi16(sd0, sd1);            \
	    __m64 sdb = _mm_unpackhi_pi16(sd0, sd1);            \
	                                                        \
	    dd = _mm_unpacklo_pi16(sda, sdb);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_42B(sd0, sd1, dd)                           \
	{                                                              \
	    __m64 sda =                                                \
		_mm_unpackhi_pi8(_mm_unpacklo_pi32(sd0, sd0), sd0);    \
	    __m64 sdb =                                                \
		_mm_unpackhi_pi8(_mm_unpacklo_pi32(sd1, sd1), sd1);    \
	    __m64 sdc = _mm_unpackhi_pi16(sda, sdb);                   \
	    __m64 sdd = _mm_unpacklo_pi16(sda, sdb);                   \
	                                                               \
	    dd = _mm_unpackhi_pi8(_mm_unpacklo_pi32(sdd, sdd), sdc);   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_42M(sd0, sd1, dd)                    \
	{                                                       \
	    __m64 sd2 = _mm_slli_pi32(sd0, 8);                  \
	    __m64 sd3 = _mm_slli_pi32(sd1, 8);                  \
	    __m64 sda = _mm_unpacklo_pi16(sd2, sd3);            \
	    __m64 sdb = _mm_unpackhi_pi16(sd2, sd3);            \
	                                                        \
	    dd = _mm_unpackhi_pi16(sda, sdb);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_42R(sd0, sd1, dd)                    \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi16(sd0, sd1);            \
	    __m64 sdb = _mm_unpackhi_pi16(sd0, sd1);            \
	                                                        \
	    dd = _mm_unpackhi_pi16(sda, sdb);                   \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_42(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *sl;
	__m64 *dp, *dl;
	__m64 dd;
	mlib_s32 j, n1, emask;

	dp = dl = (__m64 *) dst;
	sp = sl = (__m64 *) src;

	if (cmask == 3) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_U8_42R(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - (n1 << 1)));
				CHANNEL_EXTRACT_U8_42R(sp[0], sp[1], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 12) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_U8_42L(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - (n1 << 1)));
				CHANNEL_EXTRACT_U8_42L(sp[0], sp[1], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 9) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_U8_42B(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - (n1 << 1)));
				CHANNEL_EXTRACT_U8_42B(sp[0], sp[1], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_U8_42M(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - (n1 << 1)));
				CHANNEL_EXTRACT_U8_42M(sp[0], sp[1], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	}
	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_42L(sd0, sd1, dd)                   \
	{                                                       \
	    dd = _mm_unpacklo_pi32(sd0, sd1);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_42M(sd0, sd1, dd)                   \
	{                                                       \
	    __m64 sda = _mm_slli_si64(sd0, 16);                 \
	    __m64 sdb = _mm_slli_si64(sd1, 16);                 \
	                                                        \
	    dd = _mm_unpackhi_pi32(sda, sdb);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_42B(sd0, sd1, dd)                           \
	{                                                               \
	    __m64 sda = _mm_unpackhi_pi16(sd0, sd1);                    \
	    __m64 sdb = _mm_unpacklo_pi16(sd0, sd1);                    \
	                                                                \
	    dd = _mm_unpackhi_pi16(_mm_unpacklo_pi32(sdb, sdb), sda);   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_42R(sd0, sd1, dd)                   \
	{                                                       \
	    dd = _mm_unpackhi_pi32(sd0, sd1);                   \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_42(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *sl;
	__m64 *dp, *dl;
	__m64 dd;
	mlib_s32 j, n1, emask;

	dp = dl = (__m64 *) dst;
	sp = sl = (__m64 *) src;

	if (cmask == 3) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_S16_42R(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - (n1 << 1)));
				CHANNEL_EXTRACT_S16_42R(sp[0], sp[1], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 12) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_S16_42L(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - (n1 << 1)));
				CHANNEL_EXTRACT_S16_42L(sp[0], sp[1], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 9) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_S16_42B(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - (n1 << 1)));
				CHANNEL_EXTRACT_S16_42B(sp[0], sp[1], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_S16_42M(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - (n1 << 1)));
				CHANNEL_EXTRACT_S16_42M(sp[0], sp[1], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	}
	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_32R(sd0, sd1, sd2, dd0, dd1)                   \
	{                                                                 \
	    __m64 sda =                                                   \
		_mm_unpacklo_pi16(_mm_srli_si64(sd0, 8),                  \
		_mm_srli_si64(sd0, 32));                                  \
	    __m64 sdb = _mm_unpacklo_pi8(_mm_srli_si64(sd0, 56), sd1);    \
	    __m64 sdc = _mm_unpacklo_pi16(sdb, _mm_srli_si64(sd1, 16));   \
	    __m64 sdd = _mm_unpacklo_pi16(_mm_srli_si64(sd1, 40), sd2);   \
	    __m64 sde =                                                   \
		_mm_unpacklo_pi16(_mm_srli_si64(sd2, 24),                 \
		_mm_srli_si64(sd2, 48));                                  \
	    dd0 = _mm_unpacklo_pi32(sda, sdc);                            \
	    dd1 = _mm_unpacklo_pi32(sdd, sde);                            \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_32L(sd0, sd1, sd2, dd0, dd1)                   \
	{                                                                 \
	    __m64 sda = _mm_unpacklo_pi16(sd0, _mm_srli_si64(sd0, 24));   \
	    __m64 sdb =                                                   \
		_mm_unpacklo_pi16(_mm_srli_si64(sd0, 48),                 \
		_mm_srli_si64(sd1, 8));                                   \
	    __m64 sdc = _mm_unpacklo_pi8(_mm_srli_si64(sd1, 56), sd2);    \
	    __m64 sdd = _mm_unpacklo_pi16(_mm_srli_si64(sd1, 32), sdc);   \
	    __m64 sde =                                                   \
		_mm_unpacklo_pi16(_mm_srli_si64(sd2, 16),                 \
		_mm_srli_si64(sd2, 40));                                  \
	    dd0 = _mm_unpacklo_pi32(sda, sdb);                            \
	    dd1 = _mm_unpacklo_pi32(sdd, sde);                            \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_32B(sd0, sd1, sd2, dd0, dd1)                  \
	{                                                                \
	    __m64 sda = _mm_unpacklo_pi8(sd0, _mm_srl_si64(sd0, 16));    \
	    __m64 sdb =                                                  \
		_mm_unpacklo_pi8(_mm_srli_si64(sd0, 24),                 \
		_mm_srli_si64(sd0, 40));                                 \
	    __m64 sdd = _mm_unpacklo_pi8(_mm_srli_si64(sd0, 48), sd1);   \
	    __m64 sde =                                                  \
		_mm_unpacklo_pi8(_mm_srli_si64(sd1, 8),                  \
		_mm_srli_si64(sd1, 24));                                 \
	    dd0 =                                                        \
		_mm_unpacklo_pi32(_mm_unpacklo_pi16(sda, sdb),           \
		_mm_unpacklo_pi16(sdd, sde));                            \
	    sda =                                                        \
		_mm_unpacklo_pi8(_mm_srli_si64(sd1, 32),                 \
		_mm_srli_si64(sd1, 48));                                 \
	    sdb =                                                        \
		_mm_unpacklo_pi8(_mm_srli_si64(sd1, 56),                 \
		_mm_srli_si64(sd2, 8));                                  \
	    sdd =                                                        \
		_mm_unpacklo_pi8(_mm_srli_si64(sd2, 16),                 \
		_mm_srli_si64(sd2, 32));                                 \
	    sde =                                                        \
		_mm_unpacklo_pi8(_mm_srli_si64(sd2, 40),                 \
		_mm_srli_si64(sd2, 56));                                 \
	    dd1 =                                                        \
		_mm_unpacklo_pi32(_mm_unpacklo_pi16(sda, sdb),           \
		_mm_unpacklo_pi16(sdd, sde));                            \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_32(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	if (((mlib_addr)src & 7) || ((mlib_addr)dst & 7)) {
		mlib_s32 j, n1;
		mlib_u8 *dp, *dl;
		mlib_u8 *sp, *sl;

		sp = sl = (mlib_u8 *)src;
		dp = dl = (mlib_u8 *)dst;

		if (cmask == 6) {
			for (j = 0; j < ysize; j++) {
				n1 = xsize;

				for (; n1 > 4; n1 -= 4) {
					*((mlib_s16 *)(dp + 0)) =
					    *((mlib_s16 *)(sp + 0));
					*((mlib_s16 *)(dp + 2)) =
					    *((mlib_s16 *)(sp + 3));
					*((mlib_s16 *)(dp + 4)) =
					    *((mlib_s16 *)(sp + 6));
					*((mlib_s16 *)(dp + 6)) =
					    *((mlib_s16 *)(sp + 9));
					dp += 8;
					sp += 12;
				}

				for (; n1 > 0; n1--) {
					*((mlib_s16 *)(dp + 0)) =
					    *((mlib_s16 *)(sp + 0));
					dp += 2;
					sp += 3;
				}

				sp = sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
				dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
			}
		} else if (cmask == 3) {
			for (j = 0; j < ysize; j++) {
				n1 = xsize;

				for (; n1 > 4; n1 -= 4) {
					*((mlib_s16 *)(dp + 0)) =
					    *((mlib_s16 *)(sp + 1));
					*((mlib_s16 *)(dp + 2)) =
					    *((mlib_s16 *)(sp + 4));
					*((mlib_s16 *)(dp + 4)) =
					    *((mlib_s16 *)(sp + 7));
					*((mlib_s16 *)(dp + 6)) =
					    *((mlib_s16 *)(sp + 10));
					dp += 8;
					sp += 12;
				}

				for (; n1 > 0; n1--) {
					*((mlib_s16 *)(dp + 0)) =
					    *((mlib_s16 *)(sp + 1));
					dp += 2;
					sp += 3;
				}

				sp = sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
				dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			if (xsize > 0) {
				for (j = 0; j < ysize; j++) {
					n1 = xsize - 1;

					*((mlib_u8 *)(dp + 0)) =
					    *((mlib_u8 *)(sp + 0));

					dp += 1;
					sp += 2;

					for (; n1 > 4; n1 -= 4) {
						*((mlib_s16 *)(dp + 0)) =
						    *((mlib_s16 *)(sp + 0));
						*((mlib_s16 *)(dp + 2)) =
						    *((mlib_s16 *)(sp + 3));
						*((mlib_s16 *)(dp + 4)) =
						    *((mlib_s16 *)(sp + 6));
						*((mlib_s16 *)(dp + 6)) =
						    *((mlib_s16 *)(sp + 9));
						dp += 8;
						sp += 12;
					}

					for (; n1 > 0; n1--) {
						*((mlib_s16 *)(dp + 0)) =
						    *((mlib_s16 *)(sp + 0));
						dp += 2;
						sp += 3;
					}

					*((mlib_u8 *)(dp + 0)) =
					    *((mlib_u8 *)(sp + 0));

					sp = sl =
					    (mlib_u8 *)((mlib_u8 *)sl + slb);
					dp = dl =
					    (mlib_u8 *)((mlib_u8 *)dl + dlb);
				}
			}
		}
	} else {
		__m64 *sp, *sl;
		__m64 *dp, *dl;
		__m64 dd0, dd1;
		mlib_s32 j, n1, emask;

		dp = dl = (__m64 *) dst;
		sp = sl = (__m64 *) src;

		if (cmask == 6) {
			for (j = 0; j < ysize; j++) {
				for (n1 = xsize; n1 > 7; n1 -= 8) {
					CHANNEL_EXTRACT_U8_32L(sp[0], sp[1],
					    sp[2], dp[0], dp[1]);
					sp += 3;
					dp += 2;
				}

				if (n1 > 4) {
					n1 -= 4;
					emask = (0xFF << (8 - (n1 << 1)));
					CHANNEL_EXTRACT_U8_32L(sp[0], sp[1],
					    sp[2], dp[0], dd1);
					dp++;
					mmx_pst_8(dd1, dp, emask);
				} else if (n1 > 0) {
					emask = (0xFF << (8 - (n1 << 1)));
					CHANNEL_EXTRACT_U8_32L(sp[0], sp[1],
					    sp[2], dd0, dd1);
					mmx_pst_8(dd0, dp, emask);
				}

				sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
				dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
			}
		} else if (cmask == 3) {
			for (j = 0; j < ysize; j++) {
				for (n1 = xsize; n1 > 7; n1 -= 8) {
					CHANNEL_EXTRACT_U8_32R(sp[0], sp[1],
					    sp[2], dp[0], dp[1]);
					sp += 3;
					dp += 2;
				}

				if (n1 > 4) {
					n1 -= 4;
					emask = (0xFF << (8 - (n1 << 1)));
					CHANNEL_EXTRACT_U8_32R(sp[0], sp[1],
					    sp[2], dp[0], dd1);
					dp++;
					mmx_pst_8(dd1, dp, emask);
				} else if (n1 > 0) {
					emask = (0xFF << (8 - (n1 << 1)));
					CHANNEL_EXTRACT_U8_32R(sp[0], sp[1],
					    sp[2], dd0, dd1);
					mmx_pst_8(dd0, dp, emask);
				}

				sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
				dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
			}
		} else {
/*
 * SLOWER THEN PURE C
 *
 *        for (j = 0; j < ysize; j++)
 *        {
 *          for (n1 = xsize; n1 > 7; n1 -= 8)
 *          {
 *            CHANNEL_EXTRACT_U8_32B(sp[0], sp[1], sp[2], dp[0], dp[1]);
 *            sp += 3;
 *            dp += 2;
 *          }
 *
 *          if (n1 > 4)
 *          {
 *            n1 -= 4;
 *            emask = (0xFF << (8 - (n1 << 1)));
 *            CHANNEL_EXTRACT_U8_32B(sp[0], sp[1], sp[2], dp[0], dd1);
 *            dp++;
 *            mmx_pst_8(dd1, dp, emask);
 *          } else
 *          if (n1 > 0)
 *          {
 *            emask = (0xFF << (8 - (n1 << 1)));
 *            CHANNEL_EXTRACT_U8_32B(sp[0], sp[1], sp[2], dd0, dd1);
 *            mmx_pst_8(dd0, dp, emask);
 *          }
 *
 *          sp = sl = (__m64 *)((mlib_u8 *)sl + slb);
 *          dp = dl = (__m64 *)((mlib_u8 *)dl + dlb);
 *        }
 */

			if (xsize > 0) {
				mlib_u8 *dp, *dl;
				mlib_u8 *sp, *sl;

				sp = sl = (mlib_u8 *)src;
				dp = dl = (mlib_u8 *)dst;

				for (j = 0; j < ysize; j++) {
					n1 = xsize - 1;

					*((mlib_u8 *)(dp + 0)) =
					    *((mlib_u8 *)(sp + 0));

					dp += 1;
					sp += 2;

					for (; n1 > 4; n1 -= 4) {
						*((mlib_s16 *)(dp + 0)) =
						    *((mlib_s16 *)(sp + 0));
						*((mlib_s16 *)(dp + 2)) =
						    *((mlib_s16 *)(sp + 3));
						*((mlib_s16 *)(dp + 4)) =
						    *((mlib_s16 *)(sp + 6));
						*((mlib_s16 *)(dp + 6)) =
						    *((mlib_s16 *)(sp + 9));
						dp += 8;
						sp += 12;
					}

					for (; n1 > 0; n1--) {
						*((mlib_s16 *)(dp + 0)) =
						    *((mlib_s16 *)(sp + 0));
						dp += 2;
						sp += 3;
					}

					*((mlib_u8 *)(dp + 0)) =
					    *((mlib_u8 *)(sp + 0));

					sp = sl =
					    (mlib_u8 *)((mlib_u8 *)sl + slb);
					dp = dl =
					    (mlib_u8 *)((mlib_u8 *)dl + dlb);
				}
			}
		}
	}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_32L(sd0, sd1, sd2, dd0, dd1)                  \
	{                                                                 \
	    __m64 sda = _mm_unpackhi_pi16(sd0, _mm_slli_si64(sd1, 48));   \
	                                                                  \
	    dd0 = _mm_unpackhi_pi32(_mm_unpacklo_pi32(sd0, sd0), sda);    \
	    dd1 = _mm_unpackhi_pi32(sd1, _mm_slli_si64(sd2, 16));         \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_32R(sd0, sd1, sd2, dd0, dd1)                  \
	{                                                                 \
	    __m64 sda = _mm_unpackhi_pi16(sd1, _mm_slli_si64(sd2, 48));   \
	                                                                  \
	    dd0 = _mm_unpacklo_pi32(_mm_srli_si64(sd0, 16), sd1);         \
	    dd1 = _mm_unpackhi_pi32(sda, sd2);                            \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_32B(sd0, sd1, sd2, dd0, dd1)                  \
	{                                                                 \
	    __m64 sda = _mm_unpacklo_pi16(sd0, _mm_srli_si64(sd0, 48));   \
	    __m64 sdb = _mm_unpackhi_pi16(sd0, _mm_slli_si64(sd1, 16));   \
	    __m64 sdc = _mm_unpacklo_pi16(_mm_srli_si64(sd1, 32), sd2);   \
	    __m64 sdd = _mm_unpackhi_pi16(_mm_slli_si64(sd2, 32), sd2);   \
	                                                                  \
	    dd0 = _mm_unpacklo_pi16(sda, sdb);                            \
	    dd1 = _mm_unpacklo_pi32(sdc, _mm_unpackhi_pi32(sdd, sdd));    \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_32(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *sl;
	__m64 *dp, *dl;
	__m64 dd0, dd1;
	mlib_s32 j, n1, emask;

	dp = dl = (__m64 *) dst;
	sp = sl = (__m64 *) src;

	if (cmask == 6) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_32L(sp[0], sp[1], sp[2],
				    dp[0], dp[1]);
				sp += 3;
				dp += 2;
			}

			if (n1 > 2) {
				CHANNEL_EXTRACT_S16_32L(sp[0], sp[1], sp[2],
				    dp[0], dd1);
				dp++;
				mmx_pst_16(dd1, dp, (0xF << 2));
			} else if (n1 > 0) {
				emask = (0xF << (4 - (n1 << 1)));
				CHANNEL_EXTRACT_S16_32L(sp[0], sp[1], sp[2],
				    dd0, dd1);
				mmx_pst_16(dd0, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 3) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_32R(sp[0], sp[1], sp[2],
				    dp[0], dp[1]);
				sp += 3;
				dp += 2;
			}

			if (n1 > 2) {
				CHANNEL_EXTRACT_S16_32R(sp[0], sp[1], sp[2],
				    dp[0], dd1);
				dp++;
				mmx_pst_16(dd1, dp, (0xF << 2));
			} else if (n1 > 0) {
				emask = (0xF << (4 - (n1 << 1)));
				CHANNEL_EXTRACT_S16_32R(sp[0], sp[1], sp[2],
				    dd0, dd1);
				mmx_pst_16(dd0, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_32B(sp[0], sp[1], sp[2],
				    dp[0], dp[1]);
				sp += 3;
				dp += 2;
			}

			if (n1 > 2) {
				CHANNEL_EXTRACT_S16_32B(sp[0], sp[1], sp[2],
				    dp[0], dd1);
				dp++;
				mmx_pst_16(dd1, dp, (0xF << 2));
			} else if (n1 > 0) {
				emask = (0xF << (4 - (n1 << 1)));
				CHANNEL_EXTRACT_S16_32B(sp[0], sp[1], sp[2],
				    dd0, dd1);
				mmx_pst_16(dd0, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_32B(sd0, sd1, sd2, dd0, dd1)         \
	{                                                       \
	    dd0 = _mm_unpacklo_pi32(sd0, sd1);                  \
	    dd1 = _mm_unpackhi_pi32(sd1, sd2);                  \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_32R(sd0, sd1, sd2, dd0, dd1)             \
	{                                                           \
	    dd0 = _mm_unpacklo_pi32(_mm_srli_si64(sd0, 32), sd1);   \
	    dd1 = sd2;                                              \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_32L(sd0, sd1, sd2, dd0, dd1)             \
	{                                                           \
	    dd0 = sd0;                                              \
	    dd1 = _mm_unpacklo_pi32(_mm_srli_si64(sd1, 32), sd2);   \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_32_32(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *sl;
	__m64 *dp, *dl;
	__m64 dd1;
	mlib_s32 j, n1;

	dp = dl = (__m64 *) dst;
	sp = sl = (__m64 *) src;

	if (cmask == 6) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_32L(sp[0], sp[1], sp[2],
				    dp[0], dp[1]);
				sp += 3;
				dp += 2;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_32L(sp[0], sp[1], sp[2],
				    dp[0], dd1);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 3) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_32R(sp[0], sp[1], sp[2],
				    dp[0], dp[1]);
				sp += 3;
				dp += 2;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_32R(sp[0], sp[1], sp[2],
				    dp[0], dd1);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_32B(sp[0], sp[1], sp[2],
				    dp[0], dp[1]);
				sp += 3;
				dp += 2;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_32B(sp[0], sp[1], sp[2],
				    dp[0], dd1);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_42L(sd0, sd2, dd0, dd1)              \
	{                                                       \
	    dd0 = sd0;                                          \
	    dd1 = sd2;                                          \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_42M(sd0, sd1, sd2, sd3, dd0, dd1)        \
	{                                                           \
	    dd0 = _mm_unpacklo_pi32(_mm_srli_si64(sd0, 32), sd1);   \
	    dd1 = _mm_unpacklo_pi32(_mm_srli_si64(sd2, 32), sd3);   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_42B(sd0, sd1, sd2, sd3, dd0, dd1)        \
	{                                                           \
	    dd0 = _mm_unpackhi_pi32(_mm_slli_si64(sd0, 32), sd1);   \
	    dd1 = _mm_unpackhi_pi32(_mm_slli_si64(sd2, 32), sd3);   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_42R(sd1, sd3, dd0, dd1)              \
	{                                                       \
	    dd0 = sd1;                                          \
	    dd1 = sd3;                                          \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_32_42(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *sl;
	__m64 *dp, *dl;
	__m64 dd1;
	mlib_s32 j, n1;

	dp = dl = (__m64 *) dst;
	sp = sl = (__m64 *) src;

	if (cmask == 3) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42R(sp[1], sp[3], dp[0],
				    dp[1]);
				sp += 4;
				dp += 2;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42R(sp[1], sp[3], dp[0],
				    dd1);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 12) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42L(sp[0], sp[2], dp[0],
				    dp[1]);
				sp += 4;
				dp += 2;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42L(sp[0], sp[2], dp[0],
				    dd1);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 9) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42B(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1]);
				sp += 4;
				dp += 2;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42B(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dd1);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42M(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1]);
				sp += 4;
				dp += 2;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42M(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dd1);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	}
	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_D64(
    mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_d64 *sp;

/* pointer for line in src  */
	mlib_d64 *sl;

/* pointer for pixel in dst */
	mlib_d64 *dp;

/* pointer for line in dst  */
	mlib_d64 *dl;
	mlib_d64 s0, s1, s2;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;

	deltac[channeld] = 1;
	for (i = (channels - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channeld] = channels;
	for (i = 1; i < channeld; i++) {
		deltac[channeld] -= deltac[i];
	}

	sp = sl = src + deltac[0];
	dp = dl = dst;

	if (channeld == 1) {
		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				dp[i] = s0;
				sp += channels;
			}
			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channeld == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;

		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[inc0];
				dp[0] = s0;
				dp[1] = s1;
				sp += inc1;
				dp += 2;
			}
			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channeld == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;

		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[inc0];
				s2 = sp[inc1];
				dp[0] = s0;
				dp[1] = s1;
				dp[2] = s2;
				sp += inc2;
				dp += 3;
			}
			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */
