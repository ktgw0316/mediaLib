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

#ifndef _MLIB_V_VIDEODCT_VIS2_H
#define	_MLIB_V_VIDEODCT_VIS2_H

#pragma ident	"@(#)mlib_v_VideoDCT_VIS2.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/*  VIS2 Specific Defines  */
#define	TRANSPOSE_VIS2(p0, p1, p2, p3, r0, r1, r2, r3)          \
	{                                                       \
	    mlib_d64 aa0, bb0, dd0, ee0;                        \
	                                                        \
	    aa0 = vis_bshuffle(p0, p2);                         \
	    bb0 = vis_bshuffle(p1, p3);                         \
	    vis_write_bmask(0x45cd67ef, 0x0);                   \
	    dd0 = vis_bshuffle(p0, p2);                         \
	    ee0 = vis_bshuffle(p1, p3);                         \
	    r1 = vis_bshuffle(aa0, bb0);                        \
	    r3 = vis_bshuffle(dd0, ee0);                        \
	    vis_write_bmask(0x018923ab, 0x0);                   \
	    r2 = vis_bshuffle(dd0, ee0);                        \
	    r0 = vis_bshuffle(aa0, bb0);                        \
	}

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_VIDEODCT_VIS2_H */
