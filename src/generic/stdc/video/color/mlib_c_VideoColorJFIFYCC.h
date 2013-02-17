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

#ifndef _MLIB_C_VIDEOCOLORJFIFYCC_H
#define	_MLIB_C_VIDEOCOLORJFIFYCC_H

#pragma ident	"@(#)mlib_c_VideoColorJFIFYCC.h	9.3	07/10/09 SMI"

/*
 * DESCRIPTION
 *
 * Macros and tables for JFIFYCC converters
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_video.h>

extern const mlib_u8 mlib_tClip[288 * 2 + 544 * 2];

/* tables for JFIFYCC -> RGB conversions */
extern const mlib_u16 mlib_tRCr[256];
extern const mlib_u16 mlib_tGCb[256];
extern const mlib_u16 mlib_tGCr[256];
extern const mlib_u16 mlib_tBCb[256];

/* tables for RGB -> JFIFYCC conversions */
extern const mlib_u32 mlib_t_r2j[256];
extern const mlib_u32 mlib_t_g2j[256];
extern const mlib_u32 mlib_t_b2j[256];

#ifdef _LITTLE_ENDIAN

#define	SPLIT_S32_U8(x, a0, a1, a2, a3)                         \
	a3 = (x >> 24);                                         \
	a2 = (x & 0xff0000) >> 16;                              \
	a1 = (x & 0x00ff00) >> 8;                               \
	a0 = (x & 0x0000ff)

#else /* _LITTLE_ENDIAN */

#define	SPLIT_S32_U8(x, a0, a1, a2, a3)                         \
	a0 = (x >> 24);                                         \
	a1 = (x & 0xff0000) >> 16;                              \
	a2 = (x & 0x00ff00) >> 8;                               \
	a3 = (x & 0x0000ff)

#endif /* _LITTLE_ENDIAN */

#ifdef _LITTLE_ENDIAN

#define	SPLIT_S32_U8_3(x, a1, a2, a3)                           \
	a3 = (x >> 24);                                         \
	a2 = (x & 0xff0000) >> 16;                              \
	a1 = (x & 0x00ff00) >> 8

#else /* _LITTLE_ENDIAN */

#define	SPLIT_S32_U8_3(x, a1, a2, a3)                           \
	a1 = (x & 0xff0000) >> 16;                              \
	a2 = (x & 0x00ff00) >> 8;                               \
	a3 = (x & 0x0000ff)

#endif /* _LITTLE_ENDIAN */

#define	RGB2JFIFYCC_1s(rgb, s0)                                      \
	{                                                            \
	    mlib_u32 r0, g0, b0;                                     \
	                                                             \
	    r0 = (rgb)[0];                                           \
	    g0 = (rgb)[1];                                           \
	    b0 = (rgb)[2];                                           \
	    s0 = mlib_t_r2j[r0] + mlib_t_g2j[g0] + mlib_t_b2j[b0];   \
	}

#define	BGR2JFIFYCC_1s(bgr, s0)                                      \
	{                                                            \
	    mlib_u32 r0, g0, b0;                                     \
	                                                             \
	    b0 = (bgr)[0];                                           \
	    g0 = (bgr)[1];                                           \
	    r0 = (bgr)[2];                                           \
	    s0 = mlib_t_r2j[r0] + mlib_t_g2j[g0] + mlib_t_b2j[b0];   \
	}

#define	BGR2JFIFYCC_1(rgb, y0, cb0, cr0)                        \
	mlib_u32 s0;                                            \
	                                                        \
	BGR2JFIFYCC_1s(rgb, s0);                                \
	y0 = s0;                                                \
	cb0 = s0 >> 10;                                         \
	cr0 = s0 >> 21

#define	RGB2JFIFYCC_1(rgb, y0, cb0, cr0)                        \
	mlib_u32 s0;                                            \
	                                                        \
	RGB2JFIFYCC_1s(rgb, s0);                                \
	y0 = s0;                                                \
	cb0 = s0 >> 10;                                         \
	cr0 = s0 >> 21

#define	RGB2JFIFYCC_4s(rgb, s0, s1, s2, s3)                            \
	{                                                              \
	    mlib_u32 r0, g0, b0, r1, g1, b1, r2, g2, b2, r3, g3, b3;   \
	    mlib_u32 r0g0b0r1, g1b1r2g2, b2r3g3b3;                     \
	                                                               \
	    r0g0b0r1 = ((mlib_s32 *)(rgb))[0];                         \
	    g1b1r2g2 = ((mlib_s32 *)(rgb))[1];                         \
	    b2r3g3b3 = ((mlib_s32 *)(rgb))[2];                         \
	    SPLIT_S32_U8(r0g0b0r1, r0, g0, b0, r1);                    \
	    SPLIT_S32_U8(g1b1r2g2, g1, b1, r2, g2);                    \
	    SPLIT_S32_U8(b2r3g3b3, b2, r3, g3, b3);                    \
	    s0 = mlib_t_r2j[r0] + mlib_t_g2j[g0] + mlib_t_b2j[b0];     \
	    s1 = mlib_t_r2j[r1] + mlib_t_g2j[g1] + mlib_t_b2j[b1];     \
	    s2 = mlib_t_r2j[r2] + mlib_t_g2j[g2] + mlib_t_b2j[b2];     \
	    s3 = mlib_t_r2j[r3] + mlib_t_g2j[g3] + mlib_t_b2j[b3];     \
	}

#define	BGR2JFIFYCC_4s(bgr, s0, s1, s2, s3)                            \
	{                                                              \
	    mlib_u32 r0, g0, b0, r1, g1, b1, r2, g2, b2, r3, g3, b3;   \
	    mlib_u32 b0g0r0b1, g1r1b2g2, r2b3g3r3;                     \
	                                                               \
	    b0g0r0b1 = ((mlib_s32 *)(bgr))[0];                         \
	    g1r1b2g2 = ((mlib_s32 *)(bgr))[1];                         \
	    r2b3g3r3 = ((mlib_s32 *)(bgr))[2];                         \
	    SPLIT_S32_U8(b0g0r0b1, b0, g0, r0, b1);                    \
	    SPLIT_S32_U8(g1r1b2g2, g1, r1, b2, g2);                    \
	    SPLIT_S32_U8(r2b3g3r3, r2, b3, g3, r3);                    \
	    s0 = mlib_t_r2j[r0] + mlib_t_g2j[g0] + mlib_t_b2j[b0];     \
	    s1 = mlib_t_r2j[r1] + mlib_t_g2j[g1] + mlib_t_b2j[b1];     \
	    s2 = mlib_t_r2j[r2] + mlib_t_g2j[g2] + mlib_t_b2j[b2];     \
	    s3 = mlib_t_r2j[r3] + mlib_t_g2j[g3] + mlib_t_b2j[b3];     \
	}

#define	ARGB2JFIFYCC_4s(rgb, s0, s1, s2, s3)                           \
	{                                                              \
	    mlib_u32 r0, g0, b0, r1, g1, b1, r2, g2, b2, r3, g3, b3;   \
	    mlib_u32 pxl0, pxl1, pxl2, pxl3;                           \
	                                                               \
	    pxl0 = ((mlib_s32 *)(rgb))[0];                             \
	    pxl1 = ((mlib_s32 *)(rgb))[1];                             \
	    pxl2 = ((mlib_s32 *)(rgb))[2];                             \
	    pxl3 = ((mlib_s32 *)(rgb))[3];                             \
	    SPLIT_S32_U8_3(pxl0, r0, g0, b0);                          \
	    SPLIT_S32_U8_3(pxl1, r1, g1, b1);                          \
	    SPLIT_S32_U8_3(pxl2, r2, g2, b2);                          \
	    SPLIT_S32_U8_3(pxl3, r3, g3, b3);                          \
	    s0 = mlib_t_r2j[r0] + mlib_t_g2j[g0] + mlib_t_b2j[b0];     \
	    s1 = mlib_t_r2j[r1] + mlib_t_g2j[g1] + mlib_t_b2j[b1];     \
	    s2 = mlib_t_r2j[r2] + mlib_t_g2j[g2] + mlib_t_b2j[b2];     \
	    s3 = mlib_t_r2j[r3] + mlib_t_g2j[g3] + mlib_t_b2j[b3];     \
	}

#define	ABGR2JFIFYCC_4s(rgb, s0, s1, s2, s3)                           \
	{                                                              \
	    mlib_u32 r0, g0, b0, r1, g1, b1, r2, g2, b2, r3, g3, b3;   \
	    mlib_u32 pxl0, pxl1, pxl2, pxl3;                           \
	                                                               \
	    pxl0 = ((mlib_s32 *)(rgb))[0];                             \
	    pxl1 = ((mlib_s32 *)(rgb))[1];                             \
	    pxl2 = ((mlib_s32 *)(rgb))[2];                             \
	    pxl3 = ((mlib_s32 *)(rgb))[3];                             \
	    SPLIT_S32_U8_3(pxl0, b0, g0, r0);                          \
	    SPLIT_S32_U8_3(pxl1, b1, g1, r1);                          \
	    SPLIT_S32_U8_3(pxl2, b2, g2, r2);                          \
	    SPLIT_S32_U8_3(pxl3, b3, g3, r3);                          \
	    s0 = mlib_t_r2j[r0] + mlib_t_g2j[g0] + mlib_t_b2j[b0];     \
	    s1 = mlib_t_r2j[r1] + mlib_t_g2j[g1] + mlib_t_b2j[b1];     \
	    s2 = mlib_t_r2j[r2] + mlib_t_g2j[g2] + mlib_t_b2j[b2];     \
	    s3 = mlib_t_r2j[r3] + mlib_t_g2j[g3] + mlib_t_b2j[b3];     \
	}

#define	RGB2JFIFYCC_4(rgb, y0, cb0, cr0, y1, cb1, cr1, y2,      \
	cb2, cr2, y3, cb3, cr3)                                 \
	mlib_u32 s0, s1, s2, s3;                                \
	                                                        \
	RGB2JFIFYCC_4s(rgb, s0, s1, s2, s3);                    \
	y0 = s0;                                                \
	cb0 = s0 >> 10;                                         \
	cr0 = s0 >> 21;                                         \
	y1 = s1;                                                \
	cb1 = s1 >> 10;                                         \
	cr1 = s1 >> 21;                                         \
	y2 = s2;                                                \
	cb2 = s2 >> 10;                                         \
	cr2 = s2 >> 21;                                         \
	y3 = s3;                                                \
	cb3 = s3 >> 10;                                         \
	cr3 = s3 >> 21

#define	BGR2JFIFYCC_4(rgb, y0, cb0, cr0, y1, cb1, cr1, y2,      \
	cb2, cr2, y3, cb3, cr3)                                 \
	mlib_u32 s0, s1, s2, s3;                                \
	                                                        \
	BGR2JFIFYCC_4s(rgb, s0, s1, s2, s3);                    \
	y0 = s0;                                                \
	cb0 = s0 >> 10;                                         \
	cr0 = s0 >> 21;                                         \
	y1 = s1;                                                \
	cb1 = s1 >> 10;                                         \
	cr1 = s1 >> 21;                                         \
	y2 = s2;                                                \
	cb2 = s2 >> 10;                                         \
	cr2 = s2 >> 21;                                         \
	y3 = s3;                                                \
	cb3 = s3 >> 10;                                         \
	cr3 = s3 >> 21

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_VIDEOCOLORJFIFYCC_H */
