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

#pragma ident	"@(#)mlib_c_SignalLaw2Law.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_SignalALaw2uLaw - Functions in this section perform m-law
 *      and A-law compression and decompression in compliance with
 *      the ITU (former CCITT) specification.
 *
 *      mlib_SignaluLaw2ALaw - Functions in this section perform
 *      u-law and A-law compression and decompression in compliance
 *      with the ITU (former CCITT) specification.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalALaw2uLaw(mlib_u8       *ucode,
 *                                       const mlib_u8 *acode,
 *                                       mlib_s32      n);
 *      mlib_status mlib_SignaluLaw2ALaw(mlib_u8       *acode,
 *                                       const mlib_u8 *ucode,
 *                                       mlib_s32      n);
 * ARGUMENTS
 *
 *      ucode    m-law code array
 *      acode    A-law code array
 *      n        Number of samples in the input array
 *
 */

#include <mlib_signal.h>
#include <mlib_SignalCodec.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalALaw2uLaw = __mlib_SignalALaw2uLaw
#pragma weak mlib_SignaluLaw2ALaw = __mlib_SignaluLaw2ALaw

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalALaw2uLaw) mlib_SignalALaw2uLaw
    __attribute__((weak, alias("__mlib_SignalALaw2uLaw")));
__typeof__(__mlib_SignaluLaw2ALaw) mlib_SignaluLaw2ALaw
    __attribute__((weak, alias("__mlib_SignaluLaw2ALaw")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	READ_SRC_OFF0                                           \
	t0 = *(table + (s0 >> 24));                             \
	t1 = *(table + ((s0 >> 16) & 0xFF));                    \
	t2 = *(table + ((s0 >> 8) & 0xFF));                     \
	t3 = *(table + (s0 & 0xFF))

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	READ_SRC_OFF1                                           \
	t0 = *(table + (s1 & 0xFF));                            \
	t1 = *(table + (s0 >> 24));                             \
	t2 = *(table + ((s0 >> 16) & 0xFF));                    \
	t3 = *(table + ((s0 >> 8) & 0xFF))

#define	READ_SRC_OFF2                                           \
	t0 = *(table + ((s1 >> 8) & 0xFF));                     \
	t1 = *(table + (s1 & 0xFF));                            \
	t2 = *(table + (s0 >> 24));                             \
	t3 = *(table + ((s0 >> 16) & 0xFF))

#define	READ_SRC_OFF3                                           \
	t0 = *(table + ((s1 >> 16) & 0xFF));                    \
	t1 = *(table + ((s1 >> 8) & 0xFF));                     \
	t2 = *(table + (s1 & 0xFF));                            \
	t3 = *(table + (s0 >> 24))

#else /* _LITTLE_ENDIAN */

#define	READ_SRC_OFF1                                           \
	t0 = *(table + ((s0 >> 16) & 0xFF));                    \
	t1 = *(table + ((s0 >> 8) & 0xFF));                     \
	t2 = *(table + (s0 & 0xFF));                            \
	t3 = *(table + (s1 >> 24))

#define	READ_SRC_OFF2                                           \
	t0 = *(table + ((s0 >> 8) & 0xFF));                     \
	t1 = *(table + (s0 & 0xFF));                            \
	t2 = *(table + (s1 >> 24));                             \
	t3 = *(table + ((s1 >> 16) & 0xFF))

#define	READ_SRC_OFF3                                           \
	t0 = *(table + (s0 & 0xFF));                            \
	t1 = *(table + (s1 >> 24));                             \
	t2 = *(table + ((s1 >> 16) & 0xFF));                    \
	t3 = *(table + ((s1 >> 8) & 0xFF))

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

static void
mlib_c_SignalLaw2Law_SrcOff0(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    const mlib_u8 *table)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* aligned pointer to destination */
	mlib_u32 *dp;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0;

/* destination data */
	mlib_u32 t0, t1, t2;

/* destination data */
	mlib_u32 t3, acc0;

/* loop variable */
	mlib_s32 i;

	sa = (mlib_u32 *)src;
	dp = (mlib_u32 *)dst;

	s0 = (*sa++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= xsize - 8; i += 4) {
		READ_SRC_OFF0;
		acc0 = (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
		s0 = (*sa++);
		(*dp++) = acc0;
	}

	READ_SRC_OFF0;
	acc0 = (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
	(*dp++) = acc0;
	i += 4;

	sp = (mlib_u8 *)sa;
	dst = (mlib_u8 *)dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < xsize; i++) {
		(*dst++) = table[((*sp++))];
	}
}

/* *********************************************************** */

static void
mlib_c_SignalLaw2Law_SrcOff1(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    const mlib_u8 *table)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* aligned pointer to destination */
	mlib_u32 *dp;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* destination data */
	mlib_u32 t0, t1, t2;

/* destination data */
	mlib_u32 t3, acc0;

/* loop variable */
	mlib_s32 i;

	sa = (mlib_u32 *)(src - 1);
	dp = (mlib_u32 *)dst;

	s0 = (*sa++);

	s1 = (*sa++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= xsize - 8; i += 4) {
		READ_SRC_OFF1;
		acc0 = (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
		s0 = s1;
		s1 = (*sa++);
		(*dp++) = acc0;
	}

	READ_SRC_OFF1;
	acc0 = (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
	(*dp++) = acc0;
	i += 4;

	sp = (mlib_u8 *)sa;
	sp -= 3;

	dst = (mlib_u8 *)dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < xsize; i++) {
		(*dst++) = table[((*sp++))];
	}
}

/* *********************************************************** */

static void
mlib_c_SignalLaw2Law_SrcOff2(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    const mlib_u8 *table)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* aligned pointer to destination */
	mlib_u32 *dp;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* destination data */
	mlib_u32 t0, t1, t2;

/* destination data */
	mlib_u32 t3, acc0;

/* loop variable */
	mlib_s32 i;

	sa = (mlib_u32 *)(src - 2);
	dp = (mlib_u32 *)dst;

	s0 = (*sa++);

	s1 = (*sa++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= xsize - 8; i += 4) {
		READ_SRC_OFF2;
		acc0 = (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
		s0 = s1;
		s1 = (*sa++);
		(*dp++) = acc0;
	}

	READ_SRC_OFF2;
	acc0 = (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
	(*dp++) = acc0;
	i += 4;

	sp = (mlib_u8 *)sa;
	sp -= 2;

	dst = (mlib_u8 *)dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < xsize; i++) {
		(*dst++) = table[((*sp++))];
	}
}

/* *********************************************************** */

static void
mlib_c_SignalLaw2Law_SrcOff3(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    const mlib_u8 *table)
{
/* aligned pointer to source data */
	mlib_u32 *sa;

/* aligned pointer to destination */
	mlib_u32 *dp;

/* pointer to source data */
	mlib_u8 *sp;

/* source data */
	mlib_u32 s0, s1;

/* destination data */
	mlib_u32 t0, t1, t2;

/* destination data */
	mlib_u32 t3, acc0;

/* loop variable */
	mlib_s32 i;

	sa = (mlib_u32 *)(src - 3);
	dp = (mlib_u32 *)dst;

	s0 = (*sa++);

	s1 = (*sa++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= xsize - 8; i += 4) {
		READ_SRC_OFF3;
		acc0 = (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
		s0 = s1;
		s1 = (*sa++);
		(*dp++) = acc0;
	}

	READ_SRC_OFF3;
	acc0 = (t0 << 24) + (t1 << 16) + (t2 << 8) + t3;
	(*dp++) = acc0;
	i += 4;

	sp = (mlib_u8 *)sa;
	sp -= 1;

	dst = (mlib_u8 *)dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < xsize; i++) {
		(*dst++) = table[((*sp++))];
	}
}

/* *********************************************************** */

#define	FUNC_LAW_2_LAW(input, output, TABLE)                         \
	const mlib_u8 *sp = input;                                   \
	mlib_u8 *dp = output;                                        \
	mlib_s32 off, i, size = n;                                   \
	                                                             \
	if ((sp == NULL) | (dp == NULL))                             \
	    return (MLIB_FAILURE);                                   \
	if (n <= 0)                                                  \
	    return (MLIB_FAILURE);                                   \
	off = ((4 - ((mlib_addr)dp & 3)) & 3);                       \
	off = (off < size) ? off : size;                             \
	for (i = 0; i < off; i++) {                                  \
	    (*dp++) = TABLE[((*sp++))];                              \
	    size--;                                                  \
	}                                                            \
	                                                             \
	if (size >= 4) {                                             \
	    off = (mlib_addr)sp & 3;                                 \
	    if (off == 0) {                                          \
		mlib_c_SignalLaw2Law_SrcOff0(dp, sp, size, TABLE);   \
	    } else if (off == 1) {                                   \
		mlib_c_SignalLaw2Law_SrcOff1(dp, sp, size, TABLE);   \
	    } else if (off == 2) {                                   \
		mlib_c_SignalLaw2Law_SrcOff2(dp, sp, size, TABLE);   \
	    } else {                                                 \
		mlib_c_SignalLaw2Law_SrcOff3(dp, sp, size, TABLE);   \
	    }                                                        \
	} else {                                                     \
	    for (i = 0; i < size; i++) {                             \
		(*dp++) = TABLE[((*sp++))];                          \
	    }                                                        \
	}                                                            \
	                                                             \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_SignalALaw2uLaw(
    mlib_u8 *ucode,
    const mlib_u8 *acode,
    mlib_s32 n)
{
	FUNC_LAW_2_LAW(acode, ucode, mlib_a2u);
}

/* *********************************************************** */

mlib_status
__mlib_SignaluLaw2ALaw(
    mlib_u8 *acode,
    const mlib_u8 *ucode,
    mlib_s32 n)
{
	FUNC_LAW_2_LAW(ucode, acode, mlib_u2a);
}

/* *********************************************************** */
