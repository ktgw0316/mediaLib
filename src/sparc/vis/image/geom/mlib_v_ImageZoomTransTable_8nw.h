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

#ifndef _MLIB_V_IMAGEZOOMTRANSTABLE_8NW_H
#define	_MLIB_V_IMAGEZOOMTRANSTABLE_8NW_H

#pragma ident	"@(#)mlib_v_ImageZoomTransTable_8nw.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

/* mlib_v_ImageZoomTranslateTable_1_8nw.c */
void mlib_calc_4x_off_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb,
    mlib_d64 *pbuff,
    mlib_u8 *sp0);

void mlib_calc_4x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb,
    mlib_d64 *pbuff,
    mlib_u8 *sp0);

void mlib_calc_3x_off_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb,
    mlib_d64 *pbuff,
    mlib_u8 *sp0);

void mlib_calc_3x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb,
    mlib_d64 *pbuff,
    mlib_u8 *sp0);

void mlib_calc_2x_off_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb,
    mlib_d64 *pbuff,
    mlib_u8 *sp0);

void mlib_calc_2x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb,
    mlib_d64 *pbuff,
    mlib_u8 *sp0);

void mlib_calc_4y_4x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0,
    mlib_u8 *sp10,
    mlib_u8 *sp20);

void mlib_calc_4y_3x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0,
    mlib_u8 *sp10,
    mlib_u8 *sp20);

void mlib_calc_4y_2x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0,
    mlib_u8 *sp10,
    mlib_u8 *sp20);

void mlib_calc_4y_1x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0,
    mlib_u8 *sp10,
    mlib_u8 *sp20);

void mlib_calc_3y_4x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0,
    mlib_u8 *sp10);

void mlib_calc_3y_3x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0,
    mlib_u8 *sp10);

void mlib_calc_3y_2x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0,
    mlib_u8 *sp10);

void mlib_calc_3y_1x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0,
    mlib_u8 *sp10);

void mlib_calc_2y_4x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0);

void mlib_calc_2y_3x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0);

void mlib_calc_2y_2x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0);

void mlib_calc_2y_1x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0,
    mlib_u8 *spp0);

void mlib_calc_1y_4x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0);

void mlib_calc_1y_3x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0);

void mlib_calc_1y_2x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0);

void mlib_calc_1y_1x_1ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *xb,
    mlib_d64 *ybuff,
    mlib_u8 *sp0);

void mlib_calc_coef_4x_high_1ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_3x_high_1ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_2x_high_1ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_4x_1ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_3x_1ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_2x_1ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_1x_1ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

/* mlib_v_ImageZoomTranslateTable_2_8nw.c */

void mlib_calc_h_4_2ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_d64 *xb,
    mlib_u8 *sp11);

void mlib_calc_h_4_off_2ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_d64 *xb,
    mlib_u8 *sp11);

void mlib_calc_h_3_2ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_d64 *xb,
    mlib_u8 *sp11);

void mlib_calc_h_3_off_2ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_d64 *xb,
    mlib_u8 *sp11);

void mlib_calc_h_2_2ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_d64 *xb,
    mlib_u8 *sp11);

void mlib_calc_h_2_off_2ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_d64 *xb,
    mlib_u8 *sp11);

void mlib_calc_coef_4x_high_2ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_3x_high_2ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_2x_high_2ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_4x_2ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_3x_2ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_coef_2x_2ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *xb);

void mlib_calc_h1_w4_2ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *ybuff,
    mlib_d64 *xb,
    mlib_u8 *sp11);

void mlib_calc_h1_w3_2ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *ybuff,
    mlib_d64 *xb,
    mlib_u8 *sp11);

void mlib_calc_h1_w2_2ch(
    mlib_s32 x,
    mlib_zoom_workspace *ws,
    mlib_d64 *fptr,
    mlib_d64 *ybuff,
    mlib_d64 *xb,
    mlib_u8 *sp11);

/* mlib_v_ImageZoomTranslateTable_3_8nw.c */

void mlib_calc_1_4_3ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_u8 *sup0);

void mlib_calc_1_3_3ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_u8 *sup0);

void mlib_calc_1_2_3ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_u8 *sup0);

void mlib_calc_1_1_3ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_u8 *sup0);

void mlib_calc_1_4s_3ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_u8 *sup0,
    mlib_d64 dsat,
    mlib_f32 *dl);

void mlib_calc_1_3s_3ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_u8 *sup0,
    mlib_d64 dsat,
    mlib_f32 *dl);

void mlib_calc_1_2s_3ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_u8 *sup0,
    mlib_d64 dsat,
    mlib_f32 *dl);

void mlib_calc_1_1s_3ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_u8 *sup0,
    mlib_d64 dsat,
    mlib_f32 *dl);

void mlib_calc_h_4_3ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_4_off_3ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_3_3ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_3_off_3ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_2_3ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_2_off_3ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_1_3ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_1_off_3ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

/* mlib_v_ImageZoomTranslateTable_4_8nw.c */

void mlib_calc_1_4_4ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_f32 *sp0);

void mlib_calc_1_3_4ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_f32 *sp0);

void mlib_calc_1_2_4ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_f32 *sp0);

void mlib_calc_1_1_4ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_f32 *sp0);

void mlib_calc_2_4_4ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_f32 *sp0,
    mlib_f32 *sp1);

void mlib_calc_2_3_4ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_f32 *sp0,
    mlib_f32 *sp1);

void mlib_calc_2_2_4ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_f32 *sp0,
    mlib_f32 *sp1);

void mlib_calc_2_1_4ch(
    mlib_d64 *fp,
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *ybuff,
    mlib_f32 *sp0,
    mlib_f32 *sp1);

void mlib_calc_h_4_4ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_4_off_4ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_3_4ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_3_off_4ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_2_4ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_2_off_4ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_1_4ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

void mlib_calc_h_1_off_4ch(
    mlib_s32 x,
    mlib_d64 *filterX,
    mlib_zoom_workspace *ws,
    mlib_d64 *pbuff,
    mlib_f32 *sp0);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEZOOMTRANSTABLE_8NW_H */
