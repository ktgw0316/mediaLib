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

#ifndef _MLIB_V_IMAGEAFFINETABLE_8NW_H
#define	_MLIB_V_IMAGEAFFINETABLE_8NW_H

#pragma ident	"@(#)mlib_v_ImageAffineTable_8nw.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

/* mlib_v_ImageAffineTable_8nw_1ch.c */

void mlib_v_ImageAffineTableLine_8nw_4_4_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_3_4_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_2_4_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_4_2_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_3_2_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_2_2_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);

/* mlib_v_ImageAffineTable_8nw_2ch.c */

void mlib_v_ImageAffineTableLine_8nw_4_4_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_3_4_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_2_4_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_4_2_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_3_2_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_2_2_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);

/* mlib_v_ImageAffineTable_8nw_3ch.c */

void mlib_v_ImageAffineTableLine_8nw_4_4_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_3_4_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_2_4_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_4_2_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_3_2_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_2_2_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);

/* mlib_v_ImageAffineTable_8nw_4ch.c */

void mlib_v_ImageAffineTableLine_8nw_4_4_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_3_4_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_2_4_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_4_3_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_3_3_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_2_3_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);

/* mlib_v_ImageAffineTable_8nw_4ch2.c */

void mlib_v_ImageAffineTableLine_8nw_4_2_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_3_2_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);
void mlib_v_ImageAffineTableLine_8nw_2_2_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEAFFINETABLE_8NW_H */
