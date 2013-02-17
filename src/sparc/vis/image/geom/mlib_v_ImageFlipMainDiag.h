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

#ifndef _MLIB_V_IMAGEFLIPMAINDIAG_H
#define	_MLIB_V_IMAGEFLIPMAINDIAG_H

#pragma ident	"@(#)mlib_v_ImageFlipMainDiag.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

/* *********************************************************** */

#define	F_PROTO(TT)                                             \
	mlib_status                                             \
	mlib_v_ImageFlipMainDiag_##TT(void *pz, void *px,       \
	    mlib_s32 sh, mlib_s32 sw, mlib_s32 sstride,         \
	    mlib_s32 dstride)

/* *********************************************************** */

F_PROTO(U8_1);
F_PROTO(U8_2);
F_PROTO(U8_4);
F_PROTO(S16_1);
F_PROTO(S16_2);
F_PROTO(S32_1);

/* *********************************************************** */

#define	CALL_PARAMS1	p_dst, p_src, sh, sw, sstride, dstride

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEFLIPMAINDIAG_H */
