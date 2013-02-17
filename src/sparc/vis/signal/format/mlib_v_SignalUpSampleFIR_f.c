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

#pragma ident	"@(#)mlib_v_SignalUpSampleFIR_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_SignalUpSampleFIR_*
 *
 */

#include <vis_proto.h>
#include <mlib_signal.h>
#include <mlib_SignalUpSampleFIRFunc.h>

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT_S16(DST, SRC)	DST = ((mlib_s32)(SRC) >> 16)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT_S16(DST, SRC)                                       \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S16_MAX)                  \
		dsrc = (mlib_d64)MLIB_S16_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S16_MIN)                  \
		dsrc = (mlib_d64)MLIB_S16_MIN;                  \
	    DST = (mlib_s32)dsrc;                               \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SAT_F32(DST, SRC)	DST = (mlib_f32)(SRC)

/* *********************************************************** */

typedef union
{
	mlib_d64 value;
	struct
	{
		mlib_s32 int0, int1;
	} twoint;
} type_mlib_d64;

/* *********************************************************** */

void
mlib_SignalFIR_s16_to_d64(
    mlib_d64 *pdst,
    mlib_s16 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 dd;
	mlib_f32 fone = vis_to_float(0x00010001);
	type_mlib_d64 dd1, dd2;

	for (i = 0; i < n && (7 & (mlib_addr)psrc) != 0; i++)
		(*pdst++) = (mlib_d64)(*psrc++);

#pragma pipeloop(0)
	for (; i < n - 3; i += 4) {
		dd = *(mlib_d64 *)psrc;

		dd1.value = vis_fmuld8ulx16(fone, vis_read_hi(dd));
		dd2.value = vis_fmuld8ulx16(fone, vis_read_lo(dd));

		pdst[0] = (mlib_d64)dd1.twoint.int0;
		pdst[1] = (mlib_d64)dd1.twoint.int1;
		pdst[2] = (mlib_d64)dd2.twoint.int0;
		pdst[3] = (mlib_d64)dd2.twoint.int1;

		psrc += 4;
		pdst += 4;
	}

	for (; i < n; i++)
		(*pdst++) = (mlib_d64)(*psrc++);
}

/* *********************************************************** */

void
mlib_SignalFIR_f32_to_d64(
    mlib_d64 *pdst,
    mlib_f32 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		pdst[i] = psrc[i];
	}
}

/* *********************************************************** */

void
mlib_SignalFIR_d64_to_s16(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n)
{
	mlib_s32 i, j, off;
	mlib_d64 dd;
	mlib_d64 *sp0, *sp1;
	type_mlib_d64 dd1, dd2;

#ifdef MLIB_USE_FTOI_CLAMPING
	vis_write_gsr(0);
#else /* MLIB_USE_FTOI_CLAMPING */
	vis_write_gsr(16 << 3);
#endif /* MLIB_USE_FTOI_CLAMPING */

	for (i = 0; (i < n) && (7 & (mlib_addr)pdst) != 0; i++) {
		dd = *psrc;
		SAT_S16(*pdst, dd);
		pdst++;
		psrc++;
	}

#pragma pipeloop(0)
	for (; i <= n - 4; i += 4) {
		dd1.twoint.int0 = (mlib_s32)psrc[0];
		dd1.twoint.int1 = (mlib_s32)psrc[1];
		dd2.twoint.int0 = (mlib_s32)psrc[2];
		dd2.twoint.int1 = (mlib_s32)psrc[3];
		dd = vis_fpackfix_pair(dd1.value, dd2.value);

		*(mlib_d64 *)pdst = dd;

		psrc += 4;
		pdst += 4;
	}

	for (; i < n; i++) {
		SAT_S16(*pdst, *psrc);
		pdst++;
		psrc++;
	}
}

/* *********************************************************** */

void
mlib_SignalFIR_d64_to_f32(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		pdst[i] = psrc[i];
	}
}

/* *********************************************************** */

void
mlib_SignalMerge_D64_F32(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j;
	mlib_d64 *sl = psrc;
	mlib_f32 *dl = pdst;

	if (factor == 2) {
		mlib_d64 *psrc1 = psrc + n;

#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			SAT_F32(pdst[2 * i], psrc[i]);
			SAT_F32(pdst[2 * i + 1], psrc1[i]);
		}
		return;
	}

	for (j = 0; j < n; j++) {
#pragma pipeloop(0)
		for (i = 0; i < factor; i++) {
			SAT_F32(pdst[i], psrc[i * n]);
		}

		psrc = sl += 1;
		pdst = dl += factor;
	}
}

/* *********************************************************** */

void
mlib_SignalMerge_D64_F32S(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j;
	mlib_d64 *sl = psrc;
	mlib_f32 *dl = pdst;

	factor *= 2;
	n *= 2;

	if (factor == 2 * 2) {
		mlib_d64 *psrc1 = psrc + n;

#pragma pipeloop(0)
		for (i = 0; i < n; i += 2) {
			SAT_F32(pdst[2 * i], psrc[i]);
			SAT_F32(pdst[2 * i + 1], psrc[i + 1]);
			SAT_F32(pdst[2 * i + 2], psrc1[i]);
			SAT_F32(pdst[2 * i + 3], psrc1[i + 1]);
		}
		return;
	}

	for (j = 0; j < n; j += 2) {
#pragma pipeloop(0)
		for (i = 0; i < factor; i += 2) {
			SAT_F32(pdst[i], psrc[0]);
			SAT_F32(pdst[i + 1], psrc[1]);
			psrc += n;
		}

		psrc = sl += 2;
		pdst = dl += factor;
	}
}

/* *********************************************************** */

void
mlib_SignalMerge_D64_S16(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j, off;
	mlib_d64 dd;
	mlib_d64 *sp0, *sp1;
	type_mlib_d64 dd1, dd2;

#ifdef MLIB_USE_FTOI_CLAMPING
	vis_write_gsr(0);
#else /* MLIB_USE_FTOI_CLAMPING */
	vis_write_gsr(16 << 3);
#endif /* MLIB_USE_FTOI_CLAMPING */

	sp0 = psrc;

	if (factor != 2) {
		mlib_d64 *sl = psrc;
		mlib_s16 *dl = pdst;

		for (j = 0; j < n; j++) {
			for (i = 0; (i < factor) && (7 & (mlib_addr)pdst) != 0;
			    i++) {
				dd = *psrc;
				SAT_S16(*pdst, dd);
				pdst++;
				psrc += n;
			}
#pragma pipeloop(0)
			for (; i <= factor - 4; i += 4) {
				dd1.twoint.int0 = (mlib_s32)psrc[0];
				dd1.twoint.int1 = (mlib_s32)psrc[n];
				dd2.twoint.int0 = (mlib_s32)psrc[2 * n];
				dd2.twoint.int1 = (mlib_s32)psrc[3 * n];
				dd = vis_fpackfix_pair(dd1.value, dd2.value);

				*(mlib_d64 *)pdst = dd;

				psrc += 4 * n;
				pdst += 4;
			}

			for (; i < factor; i++) {
				SAT_S16(*pdst, *psrc);
				pdst++;
				psrc += n;
			}

			psrc = sl += 1;
			pdst = dl += factor;
		}
		return;
	}

	sp1 = psrc + n;
/* factor */
	n *= 2;
	off = (mlib_s32)pdst & 7;

	if (!off) {
#pragma pipeloop(0)
		for (i = 0; i <= (n - 4); i += 4) {
			dd1.twoint.int0 = (mlib_s32)sp0[0];
			dd1.twoint.int1 = (mlib_s32)sp1[0];
			dd2.twoint.int0 = (mlib_s32)sp0[1];
			dd2.twoint.int1 = (mlib_s32)sp1[1];
			dd = vis_fpackfix_pair(dd1.value, dd2.value);

			*(mlib_d64 *)pdst = dd;

			sp0 += 2;
			sp1 += 2;
			pdst += 4;
		}

		if (i < n) {
			SAT_S16(pdst[0], sp0[0]);
			SAT_S16(pdst[1], sp1[0]);
		}
	} else {
		mlib_d64 *dp = (void *)((mlib_u8 *)pdst - off);
		mlib_d64 dd_old, dd;
		mlib_s32 emask = 0xF >> (off >> 1);

		vis_alignaddrl((void *)0, off);
		i = -(off >> 1);
		dd1.twoint.int0 = (mlib_s32)sp0[0];
		dd1.twoint.int1 = (mlib_s32)sp1[0];
		dd2.twoint.int0 = (mlib_s32)sp0[1];
		dd2.twoint.int1 = (mlib_s32)sp1[1];
		dd = vis_fpackfix_pair(dd1.value, dd2.value);

		if (i <= (n - 4)) {
			dd_old = vis_faligndata(dd, dd);
			vis_pst_16(dd_old, dp, emask);
			dp++;
			dd_old = dd;
			sp0 += 2;
			sp1 += 2;
			i += 4;
#pragma pipeloop(0)
			for (; i <= (n - 4); i += 4) {
				dd1.twoint.int0 = (mlib_s32)sp0[0];
				dd1.twoint.int1 = (mlib_s32)sp1[0];
				dd2.twoint.int0 = (mlib_s32)sp0[1];
				dd2.twoint.int1 = (mlib_s32)sp1[1];
				dd = vis_fpackfix_pair(dd1.value, dd2.value);

				(*dp++) = vis_faligndata(dd_old, dd);
				dd_old = dd;

				sp0 += 2;
				sp1 += 2;
			}

			if (i < n) {
				mlib_s32 emask = 0xF0 >> (n - i);

				dd1.twoint.int0 = (mlib_s32)sp0[0];
				dd1.twoint.int1 = (mlib_s32)sp1[0];
				dd2.twoint.int0 = (mlib_s32)sp0[1];
				dd2.twoint.int1 = (mlib_s32)sp1[1];
				dd = vis_fpackfix_pair(dd1.value, dd2.value);
				dd = vis_faligndata(dd_old, dd);
				vis_pst_16(dd, dp, emask);
			}
		} else {
			mlib_s32 emask2 = 0xF0 >> (n - i);
			dd = vis_faligndata(dd_old, dd);
			vis_pst_16(dd, dp, emask & emask2);
		}
	}
}

/* *********************************************************** */

void
mlib_SignalMerge_D64_S16S(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j, off;
	mlib_d64 dd;
	mlib_d64 *sp0, *sp1;
	type_mlib_d64 dd1, dd2;

#ifdef MLIB_USE_FTOI_CLAMPING
	vis_write_gsr(0);
#else /* MLIB_USE_FTOI_CLAMPING */
	vis_write_gsr(16 << 3);
#endif /* MLIB_USE_FTOI_CLAMPING */

	sp0 = psrc;
	factor *= 2;
	n *= 2;

	off = (mlib_s32)pdst & 7;

	if (factor != 2 * 2) {
		mlib_d64 *sl = psrc;
		mlib_s16 *dl = pdst;

		for (j = 0; j < n; j += 2) {
			off = (mlib_s32)pdst & 7;

			if (!off) {
#pragma pipeloop(0)
				for (i = 0; i <= factor - 4; i += 4) {
					dd1.twoint.int0 = (mlib_s32)psrc[0];
					dd1.twoint.int1 = (mlib_s32)psrc[1];
					dd2.twoint.int0 = (mlib_s32)psrc[n];
					dd2.twoint.int1 = (mlib_s32)psrc[n + 1];
					dd = vis_fpackfix_pair(dd1.value,
					    dd2.value);

					*(mlib_d64 *)pdst = dd;

					psrc += 2 * n;
					pdst += 4;
				}

				if (i < factor) {
					SAT_S16(pdst[0], psrc[0]);
					SAT_S16(pdst[1], psrc[1]);
				}
			} else {
				mlib_d64 *dp, dd_old, dd;

				dp = (mlib_d64 *)((mlib_u8 *)pdst - off);
				vis_alignaddrl((void *)0, off);
				i = 0;

				if (factor >= 4) {
					dd1.twoint.int0 = (mlib_s32)psrc[0];
					dd1.twoint.int1 = (mlib_s32)psrc[1];
					dd2.twoint.int0 = (mlib_s32)psrc[n];
					dd2.twoint.int1 = (mlib_s32)psrc[n + 1];
					dd = vis_fpackfix_pair(dd1.value,
						dd2.value);

					dd_old = vis_faligndata(dd, dd);
					vis_pst_8(dd_old, dp, 0xFF >> off);
					dp++;
					dd_old = dd;

					psrc += 2 * n;
#pragma pipeloop(0)
					for (i = 4; i <= factor - 4; i += 4) {
					    dd1.twoint.int0 = (mlib_s32)psrc[0];
					    dd1.twoint.int1 = (mlib_s32)psrc[1];
					    dd2.twoint.int0 = (mlib_s32)psrc[n];
					    dd2.twoint.int1 = (mlib_s32)psrc[n +
						1];
					    dd = vis_fpackfix_pair(dd1.value,
						dd2.value);

					    (*dp++) = vis_faligndata(dd_old,
						dd);
					    dd_old = dd;

					    psrc += 2 * n;
					}

					vis_pst_8(vis_faligndata(dd_old,
					    dd_old), dp, 0xFF00 >> off);
				}

				if (i < factor) {
					pdst =
					    (mlib_s16 *)((mlib_u8 *)dp + off);
					SAT_S16(pdst[0], psrc[0]);
					SAT_S16(pdst[1], psrc[1]);
				}
			}

			psrc = sl += 2;
			pdst = dl += factor;
		}
		return;
	}

	sp1 = psrc + n;
/* factor */
	n *= 2;

	if (!off) {
#pragma pipeloop(0)
		for (i = 0; i <= (n - 4); i += 4) {
			dd1.twoint.int0 = (mlib_s32)sp0[0];
			dd1.twoint.int1 = (mlib_s32)sp0[1];
			dd2.twoint.int0 = (mlib_s32)sp1[0];
			dd2.twoint.int1 = (mlib_s32)sp1[1];
			dd = vis_fpackfix_pair(dd1.value, dd2.value);

			*(mlib_d64 *)pdst = dd;

			sp0 += 2;
			sp1 += 2;
			pdst += 4;
		}

		if (i < n) {
			SAT_S16(pdst[0], sp0[0]);
			SAT_S16(pdst[1], sp0[1]);
		}

	} else {
		mlib_d64 *dp = (void *)((mlib_u8 *)pdst - off);
		mlib_d64 dd_old, dd;
		mlib_s32 emask = 0xF >> (off >> 1);

		vis_alignaddrl((void *)0, off);
		i = -(off >> 1);
		dd1.twoint.int0 = (mlib_s32)sp0[0];
		dd1.twoint.int1 = (mlib_s32)sp0[1];
		dd2.twoint.int0 = (mlib_s32)sp1[0];
		dd2.twoint.int1 = (mlib_s32)sp1[1];
		dd = vis_fpackfix_pair(dd1.value, dd2.value);

		if (i <= (n - 4)) {
			dd_old = vis_faligndata(dd, dd);
			vis_pst_16(dd_old, dp, emask);
			dp++;
			dd_old = dd;
			sp0 += 2;
			sp1 += 2;
			i += 4;
#pragma pipeloop(0)
			for (; i <= (n - 4); i += 4) {
				dd1.twoint.int0 = (mlib_s32)sp0[0];
				dd1.twoint.int1 = (mlib_s32)sp0[1];
				dd2.twoint.int0 = (mlib_s32)sp1[0];
				dd2.twoint.int1 = (mlib_s32)sp1[1];
				dd = vis_fpackfix_pair(dd1.value, dd2.value);

				(*dp++) = vis_faligndata(dd_old, dd);
				dd_old = dd;

				sp0 += 2;
				sp1 += 2;
			}

			if (i < n) {
				mlib_s32 emask = 0xF0 >> (n - i);

				dd1.twoint.int0 = (mlib_s32)sp0[0];
				dd1.twoint.int1 = (mlib_s32)sp0[1];
				dd2.twoint.int0 = (mlib_s32)sp1[0];
				dd2.twoint.int1 = (mlib_s32)sp1[1];
				dd = vis_fpackfix_pair(dd1.value, dd2.value);
				dd = vis_faligndata(dd_old, dd);
				vis_pst_16(dd, dp, emask);
			}
		} else {
			mlib_s32 emask2 = 0xF0 >> (n - i);
			dd = vis_faligndata(dd_old, dd);
			vis_pst_16(dd, dp, emask & emask2);
		}
	}
}

/* *********************************************************** */
