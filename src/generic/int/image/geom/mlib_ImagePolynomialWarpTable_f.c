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

#pragma ident	"@(#)mlib_ImagePolynomialWarpTable_f.c	9.2	07/11/05 SMI"

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>
#include <mlib_GeomBasicIntOp.h>

/* *********************************************************** */

#define	DTOLL(l, d, shift)	BASIC_DTOLL(l, d, shift)

/* *********************************************************** */

#define	SAT8(out, in)                                                     \
	in -= sat;                                                        \
	out =                                                             \
	    (in >= MLIB_S32_MAX) ? MLIB_U8_MAX : (in <=                   \
	    MLIB_S32_MIN) ? MLIB_U8_MIN : ((mlib_s32)(in) >> 24) ^ 0x80

/* *********************************************************** */

#define	SAT16(out, in)                                           \
	out =                                                    \
	    (in >= MLIB_S32_MAX) ? MLIB_S16_MAX : (in <=         \
	    MLIB_S32_MIN) ? MLIB_S16_MIN : ((mlib_s32)(in)) >>   \
	    MLIB_SHIFT

/* *********************************************************** */

#define	SAT32(out, in)                                          \
	out =                                                   \
	    (in >= MLIB_S32_MAX) ? MLIB_S32_MAX : (in <=        \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(in)

/* *********************************************************** */

void
mlib_IPWT_U8_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_u8 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	/* mlib_d64 sat = (mlib_d64)0x7F800000; */
	mlib_s64 sat = 0x7F800000;
	/* mlib_d64 scale = ((mlib_d64)(1 << 24)); */
	/* mlib_d64 rr; */
	mlib_s64 rr;
	mlib_s32 i, res, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			/* rr = buffer[0] * scale; */
			DTOLL(rr, buffer[0], 24);
			SAT8(res, rr);

			dstData[idx] = res;
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_S16_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s16 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	/* mlib_d64 scale = (mlib_d64)(1 << 16); */
	/* mlib_d64 rr; */
	mlib_s64 rr;
	mlib_s32 i, res, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			/* rr = buffer[0] * scale; */
			DTOLL(rr, buffer[0], 16);
			SAT16(res, rr);

			dstData[idx] = res;
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_U16_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_u16 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	/* mlib_d64 scale = (mlib_d64)(1 << 16); */
	/* mlib_d64 rr; */
	mlib_s64 rr;
	mlib_s32 i, res, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			/* rr = (buffer[0] + MLIB_S16_MIN) * scale; */
			mlib_d64 temp;
			temp = buffer[0] + MLIB_S16_MIN;
			DTOLL(rr, temp, 16);
			SAT16(res, rr);

			dstData[idx] = res - MLIB_S16_MIN;
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_S32_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	mlib_d64 rr;
	mlib_s32 i, res, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			rr = buffer[0];
			SAT32(res, rr);
			dstData[idx] = res;
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_F32_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_f32 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	mlib_s32 i, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;

			dstData[idx] = (mlib_f32)buffer[0];
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_D64_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_d64 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	mlib_s32 i, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			dstData[idx] = buffer[0];
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarpTable_Line(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 *buffer = pws->buffer;
	mlib_IPWT_CF *call_function = pws->list_call_function;
	mlib_s32 call_count = pws->call_count;
	mlib_s32 channels = pws->channels;

	if (n <= 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n * channels; i++)
		buffer[i] = 0;

	for (i = 0; i < call_count; i++)
		call_function[i].func(buffer, lineAddr, pws, n,
		    call_function[i].offH, call_function[i].offV);

	((mlib_IPWFSat) call_function[i].func) (dstData, pws, n);
}

/* *********************************************************** */

#else /* _NO_LONGLONG */

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT8(out, in)	out = ((mlib_s32)((in) - sat) >> 24) ^ 0x80;
#define	SAT16(out, in)	out = ((mlib_s32)(in)) >> 16;

#define	SAT32(out, in)	out = (mlib_s32)(in)

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SAT8(out, in)                                                     \
	in -= sat;                                                        \
	out =                                                             \
	    (in >= MLIB_S32_MAX) ? MLIB_U8_MAX : (in <=                   \
	    MLIB_S32_MIN) ? MLIB_U8_MIN : ((mlib_s32)(in) >> 24) ^ 0x80

/* *********************************************************** */

#define	SAT16(out, in)                                           \
	out =                                                    \
	    (in >= MLIB_S32_MAX) ? MLIB_S16_MAX : (in <=         \
	    MLIB_S32_MIN) ? MLIB_S16_MIN : ((mlib_s32)(in)) >>   \
	    MLIB_SHIFT

/* *********************************************************** */

#define	SAT32(out, in)                                          \
	out =                                                   \
	    (in >= MLIB_S32_MAX) ? MLIB_S32_MAX : (in <=        \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(in)

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

void
mlib_IPWT_U8_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_u8 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	mlib_d64 sat = (mlib_d64)0x7F800000;
	mlib_d64 scale = ((mlib_d64)(1 << 24));
	mlib_d64 rr;
	mlib_s32 i, res, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			rr = buffer[0] * scale;
			SAT8(res, rr);

			dstData[idx] = res;
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_S16_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s16 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	mlib_d64 scale = (mlib_d64)(1 << 16);
	mlib_d64 rr;
	mlib_s32 i, res, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			rr = buffer[0] * scale;
			SAT16(res, rr);

			dstData[idx] = res;
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_U16_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_u16 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	mlib_d64 scale = (mlib_d64)(1 << 16);
	mlib_d64 rr;
	mlib_s32 i, res, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			rr = (buffer[0] + MLIB_S16_MIN) * scale;
			SAT16(res, rr);

			dstData[idx] = res - MLIB_S16_MIN;
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_S32_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	mlib_d64 rr;
	mlib_s32 i, res, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			rr = buffer[0];
			SAT32(res, rr);
			dstData[idx] = res;
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_F32_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_f32 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	mlib_s32 i, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;

			dstData[idx] = (mlib_f32)buffer[0];
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_IPWT_D64_Sat(
    void *sp,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_d64 *dstData = sp;
	mlib_s32 *xd = pws->dst_x;
	mlib_d64 *buffer;
	mlib_s32 i, ch;
	mlib_s32 idx;
	mlib_s32 channels = pws->channels;

	for (ch = 0; ch < channels; ch++) {
		buffer = (mlib_d64 *)pws->buffer + ch;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			idx = xd[i] * channels + ch;
			dstData[idx] = buffer[0];
			buffer += channels;
		}
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarpTable_Line(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 *buffer = pws->buffer;
	mlib_IPWT_CF *call_function = pws->list_call_function;
	mlib_s32 call_count = pws->call_count;
	mlib_s32 channels = pws->channels;

	if (n <= 0)
		return;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n * channels; i++)
		buffer[i] = 0;

	for (i = 0; i < call_count; i++)
		call_function[i].func(buffer, lineAddr, pws, n,
		    call_function[i].offH, call_function[i].offV);

	((mlib_IPWFSat) call_function[i].func) (dstData, pws, n);
}

/* *********************************************************** */

#endif /* _NO_LONGLONG */
