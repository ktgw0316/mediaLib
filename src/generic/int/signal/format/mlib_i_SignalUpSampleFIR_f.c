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

#pragma ident	"@(#)mlib_i_SignalUpSampleFIR_f.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_SignalUpSampleFIR_*
 *
 */

#include <mlib_signal.h>
#include <mlib_SignalUpSampleFIRFunc.h>
#include <mlib_Utils.h>

/* *********************************************************** */

#define	MLIB_SATURATE_S16(X)                                \
	(((X) > 0x7fff) ? MLIB_S16_MAX : ((X) < (-0x8000) ? \
	    MLIB_S16_MIN : (mlib_s16)(X)))

#define	CLAMP_F32(DST, SRC) DST = (mlib_f32)(SRC)

/* *********************************************************** */

#if !defined(_NO_LONGLONG)

void
mlib_SignalMerge_S16(
    mlib_s16 *pdst,
    mlib_s64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j;
	mlib_s64 *sl = psrc;
	mlib_s16 *dl = pdst;

	if (factor == 2) {
		mlib_s64 *psrc1 = psrc + n;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			pdst[2 * i] =
			    MLIB_SATURATE_S16(((psrc[i] + (4194304)) >> 23));
			pdst[2 * i + 1] =
			    MLIB_SATURATE_S16(((psrc1[i] + (4194304)) >> 23));
		}

		return;
	}

	for (j = 0; j < n; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i++) {
			pdst[i] = MLIB_SATURATE_S16(
			    ((psrc[i * n] + (4194304)) >> 23));
		}

		psrc = sl += 1;
		pdst = dl += factor;
	}
}

/* *********************************************************** */

void
mlib_SignalMerge_S16S(
    mlib_s16 *pdst,
    mlib_s64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j;
	mlib_s64 *sl = psrc;
	mlib_s16 *dl = pdst;

	factor *= 2;
	n *= 2;

	if (factor == 2 * 2) {
		mlib_s64 *psrc1 = psrc + n;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i += 2) {
			pdst[2 * i] =
			    MLIB_SATURATE_S16(((psrc[i] + (4194304)) >> 23));
			pdst[2 * i + 1] =
			    MLIB_SATURATE_S16(((psrc[i+1] + (4194304)) >> 23));
			pdst[2 * i + 2] =
			    MLIB_SATURATE_S16(((psrc1[i] + (4194304)) >> 23));
			pdst[2 * i + 3] =
			    MLIB_SATURATE_S16(((psrc1[i+1] + (4194304)) >> 23));
		}

		return;
	}

	for (j = 0; j < n; j += 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i += 2) {
			pdst[i] =
			    MLIB_SATURATE_S16(((psrc[0] + 4194304) >> 23));
			pdst[i + 1] =
			    MLIB_SATURATE_S16(((psrc[1] + 4194304) >> 23));
			psrc += n;
		}

		psrc = sl += 2;
		pdst = dl += factor;
	}
}

#endif /* if !defined(_NO_LONGLONG */

/* *********************************************************** */

void
mlib_SignalFIR_s16_to_d64(
    mlib_d64 *pdst,
    mlib_s16 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

#ifdef _NO_LONGLONG
	mlib_u32 data;

	if (((mlib_addr)psrc & 3) != 0) {
		(*pdst++) = (mlib_d64)(*psrc++);
		n--;
	}

	for (i = 0; i < n - 1; i += 2) {
		data = *(mlib_u32 *)psrc;

#ifdef _LITTLE_ENDIAN
		pdst[1] = (mlib_d64)(mlib_s16)(data >> 16);
		pdst[0] = (mlib_d64)(mlib_s16)(data);
#else /* _LITTLE_ENDIAN */
		pdst[0] = (mlib_d64)(mlib_s16)(data >> 16);
		pdst[1] = (mlib_d64)(mlib_s16)(data);
#endif /* _LITTLE_ENDIAN */

		psrc += 2;
		pdst += 2;
	}

	if (i < n)
		*pdst = (mlib_d64)*psrc;

#else /* _NO_LONGLONG */
	mlib_u64 data;

	for (i = 0; i < n && (7 & (mlib_addr)psrc) != 0; i++)
		(*pdst++) = (mlib_d64)(*psrc++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 3; i += 4) {
		data = *(mlib_u64 *)psrc;

#ifdef _LITTLE_ENDIAN
		pdst[3] = (mlib_d64)(mlib_s16)(data >> 48);
		pdst[2] = (mlib_d64)(mlib_s16)(data >> 32);
		pdst[1] = (mlib_d64)(mlib_s16)(data >> 16);
		pdst[0] = (mlib_d64)(mlib_s16)(data);
#else /* _LITTLE_ENDIAN */
		pdst[0] = (mlib_d64)(mlib_s16)(data >> 48);
		pdst[1] = (mlib_d64)(mlib_s16)(data >> 32);
		pdst[2] = (mlib_d64)(mlib_s16)(data >> 16);
		pdst[3] = (mlib_d64)(mlib_s16)(data);
#endif /* _LITTLE_ENDIAN */

		psrc += 4;
		pdst += 4;
	}

	for (; i < n; i++)
		(*pdst++) = (mlib_d64)(*psrc++);
#endif /* _NO_LONGLONG */
}

/* *********************************************************** */

void
mlib_SignalFIR_f32_to_d64(
    mlib_d64 *pdst,
    mlib_f32 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

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
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	for (i = 0; i < n; i++) {
		CLAMP_S16(pdst[i], psrc[i]);
	}

#else /* MLIB_USE_FTOI_CLAMPING */

#ifdef _NO_LONGLONG
	mlib_u32 data;

	if (((mlib_addr)pdst & 3) != 0) {
		(*pdst++) = ((mlib_s32)(*psrc++)) >> 16;
		n--;
	}

	for (i = 0; i < n - 1; i += 2) {
		data = ((mlib_u32)(mlib_s32)psrc[0]) & ~0xFFFF;
		data += (((mlib_u32)(mlib_s32)psrc[1]) >> 16);

		*(mlib_u32 *)pdst = data;
		psrc += 2;
		pdst += 2;
	}

	if (i < n)
		*pdst = ((mlib_s32)*psrc) >> 16;

#else /* _NO_LONGLONG */

	mlib_u64 data;

	for (i = 0; i < n && ((mlib_addr)pdst & 7) != 0; i++) {
		(*pdst++) = ((mlib_s32)(*psrc++)) >> 16;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 3; i += 4) {
		data = ((mlib_u32)(mlib_s32)psrc[0]) & ~0xFFFF;
		data += (((mlib_u32)(mlib_s32)psrc[1]) >> 16);
		data <<= 16;
		data += (((mlib_u32)(mlib_s32)psrc[2]) >> 16);
		data <<= 16;
		data += (((mlib_u32)(mlib_s32)psrc[3]) >> 16);

		*(mlib_u64 *)pdst = data;
		psrc += 4;
		pdst += 4;
	}

	for (; i < n; i++) {
		(*pdst++) = ((mlib_s32)(*psrc++)) >> 16;
	}

#endif /* _NO_LONGLONG */
#endif /* MLIB_USE_FTOI_CLAMPING */
}

/* *********************************************************** */

void
mlib_SignalFIR_d64_to_f32(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			CLAMP_F32(pdst[2 * i], psrc[i]);
			CLAMP_F32(pdst[2 * i + 1], psrc1[i]);
		}

		return;
	}

	for (j = 0; j < n; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i++) {
			CLAMP_F32(pdst[i], psrc[i * n]);
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

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i += 2) {
			CLAMP_F32(pdst[2 * i], psrc[i]);
			CLAMP_F32(pdst[2 * i + 1], psrc[i + 1]);
			CLAMP_F32(pdst[2 * i + 2], psrc1[i]);
			CLAMP_F32(pdst[2 * i + 3], psrc1[i + 1]);
		}

		return;
	}

	for (j = 0; j < n; j += 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i += 2) {
			CLAMP_F32(pdst[i], psrc[0]);
			CLAMP_F32(pdst[i + 1], psrc[1]);
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
	mlib_s32 i, j;
	mlib_d64 *sl = psrc;
	mlib_s16 *dl = pdst;

	if (factor == 2) {
		mlib_d64 *psrc1 = psrc + n;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			CLAMP_S16(pdst[2 * i], psrc[i]);
			CLAMP_S16(pdst[2 * i + 1], psrc1[i]);
		}

		return;
	}

	for (j = 0; j < n; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i++) {
			CLAMP_S16(pdst[i], psrc[i * n]);
		}

		psrc = sl += 1;
		pdst = dl += factor;
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
	mlib_s32 i, j;
	mlib_d64 *sl = psrc;
	mlib_s16 *dl = pdst;

	factor *= 2;
	n *= 2;

	if (factor == 2 * 2) {
		mlib_d64 *psrc1 = psrc + n;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i += 2) {
			CLAMP_S16(pdst[2 * i], psrc[i]);
			CLAMP_S16(pdst[2 * i + 1], psrc[i + 1]);
			CLAMP_S16(pdst[2 * i + 2], psrc1[i]);
			CLAMP_S16(pdst[2 * i + 3], psrc1[i + 1]);
		}

		return;
	}

	for (j = 0; j < n; j += 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i += 2) {
			CLAMP_S16(pdst[i], psrc[0]);
			CLAMP_S16(pdst[i + 1], psrc[1]);
			psrc += n;
		}

		psrc = sl += 2;
		pdst = dl += factor;
	}
}

/* *********************************************************** */
