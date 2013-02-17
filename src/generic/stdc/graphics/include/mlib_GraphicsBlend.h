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

#ifndef _MLIB_GRAPHICSBLEND_H
#define	_MLIB_GRAPHICSBLEND_H

#pragma ident	"@(#)mlib_GraphicsBlend.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MLIB_VIS)
#error this include file can not be used with VIS
#endif

/*
 *  MACROS
 *    MLIB_GRAPHICS_DECL_VAR_B_[8 | 32] - defines variables used in drawing with
 *                                        alpha blending
 *    MLIB_GRAPHICS_SET_VAR_B_[8 | 32]  - sets variables used in drawing with
 *                                        alpha blending
 *    MLIB_GRAPHICS_BLEND*              - perform drawing with alpha blending
 *
 *    For each drawable pixel, the original pixel value is
 *    blended with the drawing color to produce the final pixel value
 *
 *        d = (s * (255 - a) + c * a) / 255
 *
 *    where c is the color used in the drawing, a is the alpha value, s
 *    is the original value of the pixel, and d is the final value of the
 *    pixel.
 */

#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#define	MLIB_GRAPHICS_DECL_VAR_B_8				\
	mlib_d64 ca, da, a_1;

/* *********************************************************** */

#define	MLIB_GRAPHICS_SET_VAR_B_8(c, alp)			\
	da = mlib_U82D64[alp & 0xff] * (1./255.);		\
	ca = mlib_U82D64[c & 0xff] * da;			\
	a_1 = da - 1.;

/* *********************************************************** */

#define	MLIB_GRAPHICS_DECL_VAR_B_32				\
	mlib_d64 ca1, ca2, ca3, da, a_1;

/* *********************************************************** */

#define	MLIB_GRAPHICS_SET_VAR_B_32(c, alp)			\
	da = mlib_U82D64[alp & 0xff] * (1./255.);		\
	ca1 = mlib_U82D64[(c >> 16) & 0xff];			\
	ca2 = mlib_U82D64[(c >> 8) & 0xff];			\
	ca3 = mlib_U82D64[c & 0xff];				\
	ca1 *= da;						\
	ca2 *= da;						\
	ca3 *= da;						\
	a_1 = da - 1.;


/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND_8(dstp) { 			\
	mlib_d64 dsrc = mlib_U82D64[*(dstp)]; 		\
	*(dstp) = (mlib_u8)(ca - dsrc * a_1); 		\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_8(dstp, src) { 		\
	mlib_d64 dsrc = mlib_U82D64[src]; 		\
	*(dstp) = (mlib_u8)(ca - dsrc * a_1); 		\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_8_PP(dstp, srcp) {		\
	mlib_d64 dsrc = mlib_U82D64[*(srcp)];		\
	*(dstp) = (ca - dsrc * a_1);			\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_32(dst, src) { 		\
	mlib_d64 sb1 = mlib_U82D64[(src >> 16) & 0xff]; \
	mlib_d64 sb2 = mlib_U82D64[(src >> 8) & 0xff];  \
	mlib_d64 sb3 = mlib_U82D64[src & 0xff]; 	\
	mlib_s32 r0 = (mlib_s32)0xff000000;		\
	mlib_s32 r1 = (mlib_s32)(ca1 - sb1 * a_1); 	\
	mlib_s32 r2 = (mlib_s32)(ca2 - sb2 * a_1); 	\
	mlib_s32 r3 = (mlib_s32)(ca3 - sb3 * a_1); 	\
	dst = r0 | (r1 << 16) | (r2 << 8) | r3; 	\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_32_P(dstp, src) { 		\
	mlib_d64 sb1 = mlib_U82D64[(src >> 16) & 0xff]; \
	mlib_d64 sb2 = mlib_U82D64[(src >> 8) & 0xff];  \
	mlib_d64 sb3 = mlib_U82D64[src & 0xff]; 	\
	mlib_s32 r0 = (mlib_s32)0xff000000;	 	\
	mlib_s32 r1 = (mlib_s32)(ca1 - sb1 * a_1); 	\
	mlib_s32 r2 = (mlib_s32)(ca2 - sb2 * a_1); 	\
	mlib_s32 r3 = (mlib_s32)(ca3 - sb3 * a_1); 	\
	*(dstp) = r0 | (r1 << 16) | (r2 << 8) | r3; 	\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_32_PP(dstp, srcp) {	\
	mlib_s32 src = *(srcp);				\
	MLIB_GRAPHICS_BLEND2_32_P(dstp, src)		\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND_32(dstp)			\
	MLIB_GRAPHICS_BLEND2_32_P(dstp, *(dstp))

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND_8_4(dstp) { 		    \
	mlib_s32 dst = *(dstp); 			    \
	mlib_d64 sb0 = mlib_U82D64[((dst) >> 24) & 0xff];   \
	mlib_d64 sb1 = mlib_U82D64[((dst) >> 16) & 0xff];   \
	mlib_d64 sb2 = mlib_U82D64[((dst) >> 8) & 0xff];    \
	mlib_d64 sb3 = mlib_U82D64[(dst) & 0xff]; 	    \
	mlib_s32 r0 = (mlib_s32)(ca - sb0 * a_1); 	    \
	mlib_s32 r1 = (mlib_s32)(ca - sb1 * a_1); 	    \
	mlib_s32 r2 = (mlib_s32)(ca - sb2 * a_1); 	    \
	mlib_s32 r3 = (mlib_s32)(ca - sb3 * a_1); 	    \
	*(dstp) = (r0 << 24) | (r1 << 16) | (r2 << 8) | r3; \
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_GRAPHICSBLEND_H */
