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

#ifndef _MLIB_C_IMAGETHRESH1_H
#define	_MLIB_C_IMAGETHRESH1_H

#pragma ident	"@(#)mlib_c_ImageThresh1.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/* *********************************************************** */
#define	PARAMS                                                      \
	void *psrc, void *pdst, mlib_s32 src_stride,                \
	    mlib_s32 dst_stride, mlib_s32 width, mlib_s32 height,   \
	    void *__thresh, void *__ghigh, void *__glow
void mlib_c_ImageThresh1_D641(
    PARAMS);
void mlib_c_ImageThresh1_D642(
    PARAMS);
void mlib_c_ImageThresh1_D643(
    PARAMS);
void mlib_c_ImageThresh1_D644(
    PARAMS);
void mlib_c_ImageThresh1_D641_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_D642_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_D643_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_D644_1B(
    PARAMS,
    mlib_s32 dbit_off);

void mlib_c_ImageThresh1_F321(
    PARAMS);
void mlib_c_ImageThresh1_F322(
    PARAMS);
void mlib_c_ImageThresh1_F323(
    PARAMS);
void mlib_c_ImageThresh1_F324(
    PARAMS);
void mlib_c_ImageThresh1_F321_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_F322_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_F323_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_F324_1B(
    PARAMS,
    mlib_s32 dbit_off);

void mlib_c_ImageThresh1_S321(
    PARAMS);
void mlib_c_ImageThresh1_S322(
    PARAMS);
void mlib_c_ImageThresh1_S323(
    PARAMS);
void mlib_c_ImageThresh1_S324(
    PARAMS);
void mlib_c_ImageThresh1_S321_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_S322_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_S323_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_S324_1B(
    PARAMS,
    mlib_s32 dbit_off);

void mlib_c_ImageThresh1_S161(
    PARAMS);
void mlib_c_ImageThresh1_S162(
    PARAMS);
void mlib_c_ImageThresh1_S163(
    PARAMS);
void mlib_c_ImageThresh1_S164(
    PARAMS);
void mlib_c_ImageThresh1_S161_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_S162_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_S163_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_S164_1B(
    PARAMS,
    mlib_s32 dbit_off);

void mlib_c_ImageThresh1_U161(
    PARAMS);
void mlib_c_ImageThresh1_U162(
    PARAMS);
void mlib_c_ImageThresh1_U163(
    PARAMS);
void mlib_c_ImageThresh1_U164(
    PARAMS);
void mlib_c_ImageThresh1_U161_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_U162_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_U163_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_U164_1B(
    PARAMS,
    mlib_s32 dbit_off);

void mlib_c_ImageThresh1_U81(
    PARAMS);
void mlib_c_ImageThresh1_U82(
    PARAMS);
void mlib_c_ImageThresh1_U83(
    PARAMS);
void mlib_c_ImageThresh1_U84(
    PARAMS);
void mlib_c_ImageThresh1_U81_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_U82_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_U83_1B(
    PARAMS,
    mlib_s32 dbit_off);
void mlib_c_ImageThresh1_U84_1B(
    PARAMS,
    mlib_s32 dbit_off);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_IMAGETHRESH1_H */
