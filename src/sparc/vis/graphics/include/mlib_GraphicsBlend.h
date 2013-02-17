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

#pragma ident	"@(#)mlib_GraphicsBlend.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
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

#include <vis_proto.h>

/* *********************************************************** */

#define	MLIB_GRAPHICS_DECL_VAR_B_8				\
	mlib_d64 ca, da, a_1; 					\
	mlib_d64 vca;						\
	mlib_f32 va_1;						\
	mlib_s32 ia_1, ica;

/* *********************************************************** */

#define	MLIB_GRAPHICS_SET_VAR_B_8(c, alp)			\
	da = (mlib_d64)(alp & 0xff) * (1./255.);		\
	ca = (mlib_d64)(c & 0xff) * da;				\
	a_1 = da - 1.;						\
	ia_1 = (mlib_s32)(32768.0*a_1 - 0.5);			\
	va_1 = vis_to_float(ia_1); 				\
	ica = (mlib_s32)(ca * 128 + 0.5);			\
	vca = vis_to_double_dup((ica << 16) | ica);		\
	vis_write_gsr(0 << 3);

/* *********************************************************** */

#define	MLIB_GRAPHICS_DECL_VAR_B_32				\
	mlib_d64 ca1, ca2, ca3, da, a_1;			\
	mlib_d64 vca, va_1;					\
	mlib_s32 ia_1, ica1, ica2, ica3;

/* *********************************************************** */

#define	MLIB_GRAPHICS_SET_VAR_B_32(c, alp)			\
	da = (mlib_d64)(alp & 0xff) * (1./255.);		\
	ca1 = (mlib_d64)((c >> 16) & 0xff);			\
	ca2 = (mlib_d64)((c >> 8) & 0xff);			\
	ca3 = (mlib_d64)(c & 0xff);				\
	a_1 = da - 1.;						\
	da *= 128.;						\
	ca1 *= da;						\
	ca2 *= da;						\
	ca3 *= da;						\
	ia_1 = (mlib_s32)(32768.0*a_1 - 0.5);			\
	va_1 = vis_to_double((ia_1 & 0xffff), (ia_1 << 16) |	\
				(ia_1 & 0xffff));        	\
	ica1 = (mlib_s32)(ca1 + 0.5);				\
	ica2 = (mlib_s32)(ca2 + 0.5);				\
	ica3 = (mlib_s32)(ca3 + 0.5);				\
	vca = vis_to_double(0x7f800000 | ica1,			\
				(ica2 << 16) | ica3);		\
	vis_write_gsr(0 << 3);

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_32(fdst, fsrc) { 			\
	mlib_d64 dres;						\
	dres = vis_fpsub16(vca, vis_fmul8x16((fsrc), va_1));	\
	(fdst) = vis_fpack16(dres); 				\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_32_P(dstp, fsrc) { 		\
	mlib_d64 dres;						\
	dres = vis_fpsub16(vca, vis_fmul8x16(fsrc, va_1));	\
	*(mlib_f32 *)(dstp) = vis_fpack16(dres); 		\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_32_PP(dstp, srcp) {	\
	mlib_f32 fsrc = *(mlib_f32 *)(srcp);		\
	MLIB_GRAPHICS_BLEND2_32_P(dstp, fsrc)		\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND_32(dstp)				\
	MLIB_GRAPHICS_BLEND2_32_P(dstp, *(mlib_f32 *)(dstp))

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND_F(fdst, fsrc) { 			\
	mlib_d64 dres;						\
	dres = vis_fpsub16(vca, vis_fmul8x16al((fsrc), va_1));	\
	(fdst) = vis_fpack16(dres); 				\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND_8(dstp) { 				\
	mlib_d64 *dp = (mlib_d64 *)((mlib_addr)(dstp) &~ 7);	\
	mlib_f32 fsrc = *(mlib_f32 *)((mlib_addr)(dstp) &~ 3);  \
	mlib_s32 _mask = vis_edge8(dstp, dstp); 		\
	mlib_f32 fdst;  					\
	mlib_d64 _ddata;					\
	MLIB_GRAPHICS_BLEND_F(fdst, fsrc)			\
	_ddata = vis_freg_pair(fdst, fdst);			\
	vis_pst_8(_ddata, (void *)dp, _mask);			\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_8(dstp, fsrc) { 			\
	mlib_d64 *dp = (mlib_d64 *)((mlib_addr)(dstp) &~ 7); 	\
	mlib_s32 _mask = vis_edge8(dstp, dstp); 	     	\
	mlib_f32 fdst; 						\
	mlib_d64 _ddata; 					\
	MLIB_GRAPHICS_BLEND_F(fdst, fsrc) 			\
	_ddata = vis_freg_pair(fdst, fdst); 			\
	vis_pst_8(_ddata, (void *)dp, _mask);			\
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_BLEND2_8_PP(dstp, srcp) { 		\
	mlib_f32 fsrc = vis_read_lo(vis_ld_u8(srcp));		\
	mlib_f32 fdst;						\
	MLIB_GRAPHICS_BLEND_F(fdst, fsrc)			\
	*(mlib_f32 *)(dstp) = fdst;				\
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_GRAPHICSBLEND_H */
