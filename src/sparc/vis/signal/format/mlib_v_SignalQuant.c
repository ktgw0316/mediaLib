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

#pragma ident	"@(#)mlib_v_SignalQuant.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalQuant_[U8|U8S]_[S16|S16S] - 16-Bit to 8-Bit
 *                                            Quantization
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalQuant_U8S_S16S(mlib_u8        *dst,
 *                                            const mlib_s16 *src,
 *                                            const mlib_s16 *thresh,
 *                                            mlib_s32       n);
 *      mlib_status mlib_SignalQuant_U8_S16(mlib_u8        *dst,
 *                                          const mlib_s16 *src,
 *                                          const mlib_s16 *thresh,
 *                                          mlib_s32       n);
 * ARGUMENTS
 *      dst      Output signal array
 *      src      Input signal array
 *      thresh   Array of 256 thresholds
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *
 *      X(n)  = x(n), n = 0, 1, ...
 *      Z(n)  = z(n), n = 0, 1, ...
 *
 *            =   0,  x(n) < t(0)
 *            =   k,  t(k) <= x(n) < t(k+1)
 *            = 255,  x(n) >= t(255)
 */

#include <mlib_signal.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalQuant_U8S_S16S = __mlib_SignalQuant_U8S_S16S
#pragma weak mlib_SignalQuant_U8_S16 = __mlib_SignalQuant_U8_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalQuant_U8S_S16S) mlib_SignalQuant_U8S_S16S
    __attribute__((weak, alias("__mlib_SignalQuant_U8S_S16S")));
__typeof__(__mlib_SignalQuant_U8_S16) mlib_SignalQuant_U8_S16
    __attribute__((weak, alias("__mlib_SignalQuant_U8_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_u8 mlib_indexTab[512] = {
	0, 4, 0, 8, 0, 0, 0, 12,
	16, 20, 0, 24, 0, 0, 0, 28,
	0, 0, 0, 0, 0, 0, 0, 0,
	32, 36, 0, 40, 0, 0, 0, 44,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	48, 52, 0, 56, 0, 0, 0, 60,
	64, 68, 0, 72, 0, 0, 0, 76,
	80, 84, 0, 88, 0, 0, 0, 92,
	0, 0, 0, 0, 0, 0, 0, 0,
	96, 100, 0, 104, 0, 0, 0, 108,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	112, 116, 0, 120, 0, 0, 0, 124,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	128, 132, 0, 136, 0, 0, 0, 140,
	144, 148, 0, 152, 0, 0, 0, 156,
	0, 0, 0, 0, 0, 0, 0, 0,
	160, 164, 0, 168, 0, 0, 0, 172,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	176, 180, 0, 184, 0, 0, 0, 188,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	192, 196, 0, 200, 0, 0, 0, 204,
	208, 212, 0, 216, 0, 0, 0, 220,
	0, 0, 0, 0, 0, 0, 0, 0,
	224, 228, 0, 232, 0, 0, 0, 236,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	240, 244, 0, 248, 0, 0, 0, 252
};

/* *********************************************************** */

static const mlib_u8 mlib_idxShift[16] = {
	0, 0, 0, 1,
	0, 0, 0, 2,
	0, 0, 0, 0,
	0, 0, 0, 3
};

/* *********************************************************** */

#define	LOAD_ALL	dThVal = ((mlib_d64 *) thresh)[offset++]

/* *********************************************************** */

#define	LOAD_NOTALL                                             \
	mlib_d64 dsrc0, dsrc1, *dpsrc =                         \
	    (mlib_d64 *)vis_alignaddr(thresh, 0);               \
	dsrc0 = dpsrc[0];                                       \
	dsrc1 = dpsrc[1];                                       \
	dThVal = vis_faligndata(dsrc0, dsrc1);                  \
	dsrc0 = dsrc1;                                          \
	dpsrc++

/* *********************************************************** */

#define	LOAD_THRESH                                                     \
	dThVal =                                                        \
	vis_faligndata(vis_ld_u16_i((void *)thresh, offset), dThVal);   \
	offset += step;                                                 \
	dThVal =                                                        \
	vis_faligndata(vis_ld_u16_i((void *)thresh, offset), dThVal);   \
	offset += step;                                                 \
	dThVal =                                                        \
	vis_faligndata(vis_ld_u16_i((void *)thresh, offset), dThVal);   \
	offset += step;                                                 \
	dThVal =                                                        \
	vis_faligndata(vis_ld_u16_i((void *)thresh, offset), dThVal);   \
	offset += step

/* *********************************************************** */

#define	FILL_THRESH64                                           \
	offset = 0;                                             \
	step = 2 * 64;                                          \
	LOAD_THRESH;                                            \
	iThreshVal = dThVal

/* *********************************************************** */

#define	FILL_TRESH16                                            \
	offset = 0;                                             \
	step = 2 * 16;                                          \
	mask16 = 1;                                             \
	for (k = 0; k < 4; k++) {                               \
	    LOAD_THRESH;                                        \
	    thrTab16[mask16 - 1] = dThVal;                      \
	    mask16 += mask16;                                   \
	}

/* *********************************************************** */

#define	FILL_TRESH4                                             \
	offset = 0;                                             \
	step = 2 * 4;                                           \
	mask16 = 1;                                             \
	for (k = 0; k < 4; k++) {                               \
	    mask4 = 1;                                          \
	    for (i = 0; i < 4; i++) {                           \
		LOAD_THRESH;                                    \
		thrTab4[(mask16 << 3) + mask4 - 9] = dThVal;    \
		mask4 += mask4;                                 \
	    }                                                   \
	    mask16 += mask16;                                   \
	}

/* *********************************************************** */

#define	FILL_TRESH1                                                        \
	offset = 0;                                                        \
	step = 2;                                                          \
	mask16 = 1;                                                        \
	for (k = 0; k < 4; k++) {                                          \
	    mask4 = 1;                                                     \
	    for (i = 0; i < 4; i++) {                                      \
		mask1 = 1;                                                 \
		for (j = 0; j < 4; j++) {                                  \
		    LOAD_THRESH;                                           \
		    thrTab1[(mask16 << 6) + (mask4 << 3) + mask1 - 73] =   \
			dThVal;                                            \
		    mask1 += mask1;                                        \
		}                                                          \
		mask4 += mask4;                                            \
	    }                                                              \
	    mask16 += mask16;                                              \
	}

/* *********************************************************** */

#define	SRC1x4(dsrc)                                                \
	dsrc = vis_fpmerge(vis_read_lo(dsrc), vis_read_lo(dsrc));   \
	dsrc = vis_fpmerge(vis_read_lo(dsrc), vis_read_lo(dsrc));   \
	dsrc = vis_fpmerge(vis_read_hi(dsrc), vis_read_lo(dsrc))

/* *********************************************************** */

#define	SRC2x8(dsrc0, dsrc1, fsrc)                                         \
	{                                                                  \
	    mlib_d64 dsrc = vis_fpmerge(fsrc, fsrc);                       \
	                                                                   \
	    dsrc0 = vis_fpmerge(vis_read_hi(dsrc), vis_read_hi(dsrc));     \
	    dsrc0 = vis_fpmerge(vis_read_hi(dsrc0), vis_read_lo(dsrc0));   \
	    dsrc1 = vis_fpmerge(vis_read_lo(dsrc), vis_read_lo(dsrc));     \
	    dsrc1 = vis_fpmerge(vis_read_hi(dsrc1), vis_read_lo(dsrc1));   \
	}

/* *********************************************************** */

#define	LOAD_S16(ind)	*((mlib_s16 *) ((mlib_u8 *) thresh + ind))

/* *********************************************************** */

mlib_status
__mlib_SignalQuant_U8_S16(
    mlib_u8 *dst,
    const mlib_s16 *src,
    const mlib_s16 *thresh,
    mlib_s32 n)
{
	mlib_u16 *pSrc = (mlib_u16 *)src;
	mlib_f32 *fpSrc, fSrc;
	mlib_d64 iThreshVal, dThVal = 0, dSrc0, dSrc1, dSrc2, dSrc3;
	mlib_d64 thrTab16[8], thrTab4[64], thrTab1[512];
	mlib_s32 mask64, mask16, mask4, mask1;
	mlib_s32 i, j, k, offset, step, idx;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (dst == NULL || src == NULL || thresh == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 64) {
		for (i = 0; i < n; i++) {
			k = 384 + (((src[i] - LOAD_S16(256)) >> 31) & ~0xff);
			k += 64 + (((src[i] - LOAD_S16(k)) >> 31) & ~0x7f);
			k += 32 + (((src[i] - LOAD_S16(k)) >> 31) & ~0x3f);
			k += 16 + (((src[i] - LOAD_S16(k)) >> 31) & ~0x1f);
			k += 8 + (((src[i] - LOAD_S16(k)) >> 31) & ~0xf);
			k += 4 + (((src[i] - LOAD_S16(k)) >> 31) & ~7);
			k += 2 + (((src[i] - LOAD_S16(k)) >> 31) & ~3);
			dst[i] = (k >> 1) + ((src[i] - LOAD_S16(k)) >> 31);
		}

		return (MLIB_SUCCESS);
	}

	vis_alignaddr(0, 6);
	FILL_THRESH64;
	FILL_TRESH16;
	FILL_TRESH4;
	FILL_TRESH1;

	for (i = 0; i < n && ((mlib_addr)pSrc & 3) != 0; i++, pSrc++) {
		dSrc0 = vis_ld_u16_i(pSrc, 0);
		SRC1x4(dSrc0);

		mask64 = (vis_fcmple16(iThreshVal, dSrc0) >> 1);
		dThVal = thrTab16[mask64];

		mask16 = (vis_fcmple16(dThVal, dSrc0) >> 1);
		dThVal = thrTab4[(mask64 << 3) + mask16];

		mask4 = (vis_fcmple16(dThVal, dSrc0) >> 1);
		dThVal = thrTab1[(mask64 << 6) + (mask16 << 3) + mask4];

		idx =
		    (mlib_s32)mlib_indexTab[(mask64 << 6) + (mask16 << 3) +
		    mask4];
		mask1 = vis_fcmple16(dThVal, dSrc0);
		(*dst++) = idx + mlib_idxShift[mask1];
	}

	fpSrc = (mlib_f32 *)pSrc;
	fSrc = vis_ld_f32_nf(fpSrc); fpSrc++;
	SRC2x8(dSrc0, dSrc1, fSrc);

	for (; i <= n - 2; i += 2) {
		fSrc = vis_ld_f32_nf(fpSrc); fpSrc++;
		SRC2x8(dSrc2, dSrc3, fSrc);

		mask64 = (vis_fcmple16(iThreshVal, dSrc0) >> 1);
		dThVal = thrTab16[mask64];

		mask16 = (vis_fcmple16(dThVal, dSrc0) >> 1);
		mask64 = (mask64 << 3) + mask16;

		dThVal = thrTab4[mask64];
		mask4 = (vis_fcmple16(dThVal, dSrc0) >> 1);

		mask64 = (mask64 << 3) + mask4;
		dThVal = thrTab1[mask64];
		idx = mlib_indexTab[mask64];

		mask1 = vis_fcmple16(dThVal, dSrc0);
		(*dst++) = idx + mlib_idxShift[mask1];
		dSrc0 = dSrc2;

		mask64 = (vis_fcmple16(iThreshVal, dSrc1) >> 1);
		dThVal = thrTab16[mask64];

		mask16 = (vis_fcmple16(dThVal, dSrc1) >> 1);
		mask64 = (mask64 << 3) + mask16;

		dThVal = thrTab4[mask64];
		mask4 = (vis_fcmple16(dThVal, dSrc1) >> 1);

		mask64 = (mask64 << 3) + mask4;
		dThVal = thrTab1[mask64];
		idx = mlib_indexTab[mask64];

		mask1 = vis_fcmple16(dThVal, dSrc1);
		(*dst++) = idx + mlib_idxShift[mask1];
		dSrc1 = dSrc3;
	}

	for (; i < n; i++) {
		dSrc0 = vis_ld_u16_i((void *)src, 2 * i);
		SRC1x4(dSrc0);

		mask64 = (vis_fcmple16(iThreshVal, dSrc0) >> 1);
		dThVal = thrTab16[mask64];

		mask16 = (vis_fcmple16(dThVal, dSrc0) >> 1);
		dThVal = thrTab4[(mask64 << 3) + mask16];

		mask4 = (vis_fcmple16(dThVal, dSrc0) >> 1);
		dThVal = thrTab1[(mask64 << 6) + (mask16 << 3) + mask4];

		idx =
		    (mlib_s32)mlib_indexTab[(mask64 << 6) + (mask16 << 3) +
		    mask4];
		mask1 = vis_fcmple16(dThVal, dSrc0);
		(*dst++) = idx + mlib_idxShift[mask1];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalQuant_U8S_S16S(
    mlib_u8 *dst,
    const mlib_s16 *src,
    const mlib_s16 *thresh,
    mlib_s32 n)
{
	return (__mlib_SignalQuant_U8_S16(dst, src, thresh, 2 * n));
}

/* *********************************************************** */
