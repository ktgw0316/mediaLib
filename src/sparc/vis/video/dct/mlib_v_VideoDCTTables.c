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

#pragma ident	"@(#)mlib_v_VideoDCTTables.c	9.2	07/11/05 SMI"

#include <mlib_video.h>

#define	__ENABLE_DECLARE_ALIGN

#include <mlib_v_VideoDCT.h>

/* *********************************************************** */

const mlib_u32 mlib_ccf16[] = {
	0x20002c63, 0x29cf25a1, 0x200025a1, 0x29cf2c63,
	0x2c633d90, 0x39fe3431, 0x2c633431, 0x39fe3d90,
	0x29cf39fe, 0x36a1312a, 0x29cf312a, 0x36a139fe,
	0x25a13431, 0x312a2c3f, 0x25a12c3f, 0x312a3431
};

const mlib_u32 mlib_ccf16_12[] = {
	0x400058c5, 0x539f4b41, 0x40004b41, 0x539f58c5,
	0x58c57b21, 0x73fc6862, 0x58c56862, 0x73fc7b21,
	0x539f73fc, 0x6d416254, 0x539f6254, 0x6d4173fc,
	0x4b416862, 0x6254587e, 0x4b42587e, 0x62546862
};

const mlib_u32 mlib_ccf6[] = {
	0xb5b5b5b5, 0x33333333,
	0x6a6a6a6a, 0xabababab,
	0x00000100, 0x40404040
};

const mlib_u16 mlib_cTable[] = {
	0xfbfb, 0xfbfb, 0xecec, 0xecec,
	0x6262, 0x6262, 0x3232, 0x3232,
	0xb5b5, 0xb5b5, 0x4040, 0x4040,
	0x0004, 0x0000, 0x0000, 0x0000
};

/* *********************************************************** */
