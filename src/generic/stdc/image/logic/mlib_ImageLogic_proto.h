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

#ifndef _MLIB_IMAGELOGIC_PROTO_H
#define	_MLIB_IMAGELOGIC_PROTO_H

#pragma ident	"@(#)mlib_ImageLogic_proto.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

mlib_status mlib_ImageAnd_Bit(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);
mlib_status mlib_ImageAndNot_Bit(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);
mlib_status mlib_ImageNot_Bit(
    mlib_image *dst,
    const mlib_image *src);
mlib_status mlib_ImageNotAnd_Bit(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);
mlib_status mlib_ImageNotOr_Bit(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);
mlib_status mlib_ImageNotXor_Bit(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);
mlib_status mlib_ImageOr_Bit(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);
mlib_status mlib_ImageOrNot_Bit(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);
mlib_status mlib_ImageXor_Bit(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);

mlib_status mlib_ImageConstAnd_Bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);
mlib_status mlib_ImageConstAndNot_Bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);
mlib_status mlib_ImageConstNotAnd_Bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);
mlib_status mlib_ImageConstNotOr_Bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);
mlib_status mlib_ImageConstNotXor_Bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);
mlib_status mlib_ImageConstOr_Bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);
mlib_status mlib_ImageConstOrNot_Bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);
mlib_status mlib_ImageConstXor_Bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGELOGIC_PROTO_H */
