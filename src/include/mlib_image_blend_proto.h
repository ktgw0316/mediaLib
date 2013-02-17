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

#ifndef _MLIB_IMAGE_BLEND_PROTO_H
#define	_MLIB_IMAGE_BLEND_PROTO_H

#pragma ident	"@(#)mlib_image_blend_proto.h	9.3	07/10/09 SMI"

#include <mlib_types.h>
#include <mlib_status.h>
#include <mlib_image_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *    NOTE: f = min(ALPHAsrc2, 1 - ALPHAsrc1)
 *          f = min(ALPHAscr2, 1 - ALPHAsrc1dst) for In-place function
 *          ALPHA = (ALPHA, ALPHA, ALPHA, ALPHA)
 */

/* dst = 0 */
mlib_status mlib_ImageBlend_ZERO_ZERO(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = 0 */
mlib_status mlib_ImageBlend_ZERO_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 */
mlib_status mlib_ImageBlend_ZERO_ONE(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 */
mlib_status mlib_ImageBlend_ZERO_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * src1 */
mlib_status mlib_ImageBlend_ZERO_DC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * src1dst */
mlib_status mlib_ImageBlend_ZERO_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * (1 - src1) */
mlib_status mlib_ImageBlend_ZERO_OMDC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * (1 - src1dst) */
mlib_status mlib_ImageBlend_ZERO_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * ALPHAsrc2 */
mlib_status mlib_ImageBlend_ZERO_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * ALPHAsrc2 */
mlib_status mlib_ImageBlend_ZERO_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_ZERO_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_ZERO_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * ALPHAsrc1 */
mlib_status mlib_ImageBlend_ZERO_DA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * ALPHAsrc1dst */
mlib_status mlib_ImageBlend_ZERO_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * (1 - ALPHAsrc1) */
mlib_status mlib_ImageBlend_ZERO_OMDA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * (1 - ALPHAsrc1dst) */
mlib_status mlib_ImageBlend_ZERO_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_ZERO_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_ZERO_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 */
mlib_status mlib_ImageBlend_ONE_ZERO(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst */
mlib_status mlib_ImageBlend_ONE_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 */
mlib_status mlib_ImageBlend_ONE_ONE(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 */
mlib_status mlib_ImageBlend_ONE_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 + src2) */
mlib_status mlib_ImageBlend_ONE_DC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 + src2) */
mlib_status mlib_ImageBlend_ONE_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 + src1 * (1 - src2) */
mlib_status mlib_ImageBlend_ONE_OMDC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 + src1dst * (1 - src2) */
mlib_status mlib_ImageBlend_ONE_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 * ALPHAsrc2 */
mlib_status mlib_ImageBlend_ONE_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 * ALPHAsrc2 */
mlib_status mlib_ImageBlend_ONE_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_ONE_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_ONE_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 * ALPHAsrc1 */
mlib_status mlib_ImageBlend_ONE_DA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 * ALPHAsrc1dst */
mlib_status mlib_ImageBlend_ONE_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 * (1 - ALPHAsrc1) */
mlib_status mlib_ImageBlend_ONE_OMDA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 * (1 - ALPHAsrc1dst) */
mlib_status mlib_ImageBlend_ONE_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_ONE_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_ONE_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * src2 */
mlib_status mlib_ImageBlend_SC_ZERO(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * src2 */
mlib_status mlib_ImageBlend_SC_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = (src1 + 1) * src2 */
mlib_status mlib_ImageBlend_SC_ONE(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = (src1dst + 1) * src2 */
mlib_status mlib_ImageBlend_SC_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = 2 * src1 * src2 */
mlib_status mlib_ImageBlend_SC_DC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = 2 * src1dst * src2 */
mlib_status mlib_ImageBlend_SC_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 */
mlib_status mlib_ImageBlend_SC_OMDC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 */
mlib_status mlib_ImageBlend_SC_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * (src1 + ALPHAsrc2) */
mlib_status mlib_ImageBlend_SC_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * (src1dst + ALPHAsrc2) */
mlib_status mlib_ImageBlend_SC_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * (1 - ALPHAsrc2 + src1) */
mlib_status mlib_ImageBlend_SC_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * (1 - ALPHAsrc2 + src1dst) */
mlib_status mlib_ImageBlend_SC_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * (src1 + ALPHAsrc1) */
mlib_status mlib_ImageBlend_SC_DA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * (src1dst + ALPHAsrc1dst) */
mlib_status mlib_ImageBlend_SC_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * (1 - ALPHAsrc1 + src1) */
mlib_status mlib_ImageBlend_SC_OMDA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * (1 - ALPHAsrc1dst + src1dst) */
mlib_status mlib_ImageBlend_SC_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src2 * ((f, f, f, 1) + src1) */
mlib_status mlib_ImageBlend_SC_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src2 * ((f, f, f, 1) + src1dst) */
mlib_status mlib_ImageBlend_SC_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - src2) */
mlib_status mlib_ImageBlend_OMSC_ZERO(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - src2) */
mlib_status mlib_ImageBlend_OMSC_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 * (1 - src1) */
mlib_status mlib_ImageBlend_OMSC_ONE(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 * (1 - src1dst) */
mlib_status mlib_ImageBlend_OMSC_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 */
mlib_status mlib_ImageBlend_OMSC_DC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst */
mlib_status mlib_ImageBlend_OMSC_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 - 2 * src1 * src2 */
mlib_status mlib_ImageBlend_OMSC_OMDC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 - 2 * src1dst * src2 */
mlib_status mlib_ImageBlend_OMSC_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 * (ALPHAsrc2 - src1) */
mlib_status mlib_ImageBlend_OMSC_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 * (ALPHAsrc2 - src1dst) */
mlib_status mlib_ImageBlend_OMSC_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 - src2 * (src1 + ALPHAsrc2) */
mlib_status mlib_ImageBlend_OMSC_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 - src2 * (src1dst + ALPHAsrc2) */
mlib_status mlib_ImageBlend_OMSC_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 * (ALPHAsrc1 - src1) */
mlib_status mlib_ImageBlend_OMSC_DA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 * (ALPHAsrc1dst - src1dst) */
mlib_status mlib_ImageBlend_OMSC_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + src2 - src2 * (src1 + ALPHAsrc1) */
mlib_status mlib_ImageBlend_OMSC_OMDA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + src2 - src2 * (src1dst + ALPHAsrc1dst) */
mlib_status mlib_ImageBlend_OMSC_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 +  src2 * ((f, f, f, 1) - src1) */
mlib_status mlib_ImageBlend_OMSC_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst +  src2 * ((f, f, f, 1) - src1dst) */
mlib_status mlib_ImageBlend_OMSC_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc2 */
mlib_status mlib_ImageBlend_SA_ZERO(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc2 */
mlib_status mlib_ImageBlend_SA_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc2 + src2 */
mlib_status mlib_ImageBlend_SA_ONE(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc2 + src2 */
mlib_status mlib_ImageBlend_SA_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (ALPHAsrc2 + src2) */
mlib_status mlib_ImageBlend_SA_DC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (ALPHAsrc2 + src2) */
mlib_status mlib_ImageBlend_SA_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (ALPHAsrc2 - src2) + src2 */
mlib_status mlib_ImageBlend_SA_OMDC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (ALPHAsrc2 - src2) + src2 */
mlib_status mlib_ImageBlend_SA_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = (src1 + src2) * ALPHAsrc2 */
mlib_status mlib_ImageBlend_SA_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = (src1dst + src2) * ALPHAsrc2 */
mlib_status mlib_ImageBlend_SA_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = (src1 - src2) * ALPHAsrc2 + src2 */
mlib_status mlib_ImageBlend_SA_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = (src1dst - src2) * ALPHAsrc2 + src2 */
mlib_status mlib_ImageBlend_SA_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc2 + src2 * ALPHAsrc1 */
mlib_status mlib_ImageBlend_SA_DA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc2 + src2 * ALPHAsrc1dst */
mlib_status mlib_ImageBlend_SA_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc2 + src2 * (1 - ALPHAsrc1) */
mlib_status mlib_ImageBlend_SA_OMDA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc2 + src2 * (1 - ALPHAsrc1dst) */
mlib_status mlib_ImageBlend_SA_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc2 + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_SA_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc2 + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_SA_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_OMSA_ZERO(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_OMSA_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc2) + src2 */
mlib_status mlib_ImageBlend_OMSA_ONE(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc2) + src2 */
mlib_status mlib_ImageBlend_OMSA_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc2 + src2) */
mlib_status mlib_ImageBlend_OMSA_DC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc2 + src2) */
mlib_status mlib_ImageBlend_OMSA_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc2 - src2) + src2 */
mlib_status mlib_ImageBlend_OMSA_OMDC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc2 - src2) + src2 */
mlib_status mlib_ImageBlend_OMSA_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + (src2 - src1) * ALPHAsrc2 */
mlib_status mlib_ImageBlend_OMSA_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + (src2 - src1dst) * ALPHAsrc2 */
mlib_status mlib_ImageBlend_OMSA_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = (src1 + src2) * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_OMSA_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = (src1dst + src2) * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_OMSA_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc2) + src2 * ALPHAsrc1 */
mlib_status mlib_ImageBlend_OMSA_DA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc2) + src2 * ALPHAsrc1dst */
mlib_status mlib_ImageBlend_OMSA_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc2) + src2 * (1 - ALPHAsrc1) */
mlib_status mlib_ImageBlend_OMSA_OMDA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc2) + src2 * (1 - ALPHAsrc1dst) */
mlib_status mlib_ImageBlend_OMSA_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc2) + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_OMSA_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc2) + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_OMSA_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc1 */
mlib_status mlib_ImageBlend_DA_ZERO(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc1dst */
mlib_status mlib_ImageBlend_DA_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc1 + src2 */
mlib_status mlib_ImageBlend_DA_ONE(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc1dst + src2 */
mlib_status mlib_ImageBlend_DA_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (ALPHAsrc1 + src2) */
mlib_status mlib_ImageBlend_DA_DC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (ALPHAsrc1dst + src2) */
mlib_status mlib_ImageBlend_DA_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (ALPHAsrc1 - src2) + src2 */
mlib_status mlib_ImageBlend_DA_OMDC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (ALPHAsrc1dst - src2) + src2 */
mlib_status mlib_ImageBlend_DA_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc1 + src2 * ALPHAsrc2 */
mlib_status mlib_ImageBlend_DA_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc1dst + src2 * ALPHAsrc2 */
mlib_status mlib_ImageBlend_DA_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc1 + src2 * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_DA_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc1dst + src2 * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_DA_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = (src1 + src2) * ALPHAsrc1 */
mlib_status mlib_ImageBlend_DA_DA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = (src1dst + src2) * ALPHAsrc1dst */
mlib_status mlib_ImageBlend_DA_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = (src1 - src2) * ALPHAsrc1 + src2 */
mlib_status mlib_ImageBlend_DA_OMDA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = (src1dst - src2) * ALPHAsrc1dst + src2 */
mlib_status mlib_ImageBlend_DA_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * ALPHAsrc1 + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_DA_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * ALPHAsrc1dst + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_DA_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc1) */
mlib_status mlib_ImageBlend_OMDA_ZERO(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc1dst) */
mlib_status mlib_ImageBlend_OMDA_ZERO_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc1) + src2 */
mlib_status mlib_ImageBlend_OMDA_ONE(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 */
mlib_status mlib_ImageBlend_OMDA_ONE_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc1 + src2) */
mlib_status mlib_ImageBlend_OMDA_DC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc1dst + src2) */
mlib_status mlib_ImageBlend_OMDA_DC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc1 - src2) + src2 */
mlib_status mlib_ImageBlend_OMDA_OMDC(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc1dst - src2) + src2 */
mlib_status mlib_ImageBlend_OMDA_OMDC_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc1) + src2 * ALPHAsrc2 */
mlib_status mlib_ImageBlend_OMDA_SA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * ALPHAsrc2 */
mlib_status mlib_ImageBlend_OMDA_SA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc1) + src2 * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_OMDA_OMSA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * (1 - ALPHAsrc2) */
mlib_status mlib_ImageBlend_OMDA_OMSA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 + (src2 - src1) * ALPHAsrc1 */
mlib_status mlib_ImageBlend_OMDA_DA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst + (src2 - src1dst) * ALPHAsrc1dst */
mlib_status mlib_ImageBlend_OMDA_DA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = (src1 + src2) * (1 - ALPHAsrc1) */
mlib_status mlib_ImageBlend_OMDA_OMDA(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = (src1dst + src2) * (1 - ALPHAsrc1dst) */
mlib_status mlib_ImageBlend_OMDA_OMDA_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

/* dst = src1 * (1 - ALPHAsrc1) + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_OMDA_SAS(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 cmask);

/* src1dst = src1dst * (1 - ALPHAsrc1dst) + src2 * (f, f, f, 1) */
mlib_status mlib_ImageBlend_OMDA_SAS_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_s32 cmask);

mlib_status mlib_ImageComposite(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_blend bsrc1,
    mlib_blend bsrc2,
    mlib_s32 cmask);

mlib_status mlib_ImageComposite_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    mlib_blend bsrc1,
    mlib_blend bsrc2,
    mlib_s32 cmask);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGE_BLEND_PROTO_H */
