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

#pragma ident   "@(#)mlib_i_ImageArith_Div_Tab_U8.c	9.2    07/11/05 SMI"

/*
 * DESCRIPTION:
 *
 *  mlib_s64 mlib_ImageArith_DivAlpha_U8[x] = (1u << 31) / x;
 *  mlib_s64 mlib_ImageArith_DivShift_U8[x] = (1u << 16) / x;
 *  mlib_s64 mlib_ImageArith_ConstDiv_U8[x] = (1u << 16) / x;
 *
 * REFERENCING:
 *  mlib_ImageArith_DivAlpha_U8 is used to exclude dividing in functions
 *  	mlib_ImageDivAlpha_U8
 *  	mlib_ImageDivAlpha_Inp_U8
 *
 *  mlib_ImageArith_DivShif_U8t is used to exclude dividing in function
 *   	mlib_ImageDivShift_U8
 *  	mlib_ImageDivShift_Inp_U8
 *
 *  mlib_ImageArith_ConstDiv_U8 is used to exclude dividing in function
 *  	mlib_ImageConstDiv_U8
 *  	mlib_ImageConstDiv_Inp_U8
 */

#include <mlib_types.h>

#define	__IMAGEARITH_DIV_TAB_U8_DEFINED

#include <mlib_i_ImageArith_Div_Tab_U8.h>

/* *********************************************************** */

const mlib_s64 mlib_ImageArith_DivAlpha_U8[] = {
1u << 31,
(1u << 31) / 1, (1u << 31) / 2, (1u << 31) / 3,
(1u << 31) / 4, (1u << 31) / 5, (1u << 31) / 6, (1u << 31) / 7,
(1u << 31) / 8, (1u << 31) / 9, (1u << 31) / 10, (1u << 31) / 11,
(1u << 31) / 12, (1u << 31) / 13, (1u << 31) / 14, (1u << 31) / 15,
(1u << 31) / 16, (1u << 31) / 17, (1u << 31) / 18, (1u << 31) / 19,
(1u << 31) / 20, (1u << 31) / 21, (1u << 31) / 22, (1u << 31) / 23,
(1u << 31) / 24, (1u << 31) / 25, (1u << 31) / 26, (1u << 31) / 27,
(1u << 31) / 28, (1u << 31) / 29, (1u << 31) / 30, (1u << 31) / 31,
(1u << 31) / 32, (1u << 31) / 33, (1u << 31) / 34, (1u << 31) / 35,
(1u << 31) / 36, (1u << 31) / 37, (1u << 31) / 38, (1u << 31) / 39,
(1u << 31) / 40, (1u << 31) / 41, (1u << 31) / 42, (1u << 31) / 43,
(1u << 31) / 44, (1u << 31) / 45, (1u << 31) / 46, (1u << 31) / 47,
(1u << 31) / 48, (1u << 31) / 49, (1u << 31) / 50, (1u << 31) / 51,
(1u << 31) / 52, (1u << 31) / 53, (1u << 31) / 54, (1u << 31) / 55,
(1u << 31) / 56, (1u << 31) / 57, (1u << 31) / 58, (1u << 31) / 59,
(1u << 31) / 60, (1u << 31) / 61, (1u << 31) / 62, (1u << 31) / 63,
(1u << 31) / 64, (1u << 31) / 65, (1u << 31) / 66, (1u << 31) / 67,
(1u << 31) / 68, (1u << 31) / 69, (1u << 31) / 70, (1u << 31) / 71,
(1u << 31) / 72, (1u << 31) / 73, (1u << 31) / 74, (1u << 31) / 75,
(1u << 31) / 76, (1u << 31) / 77, (1u << 31) / 78, (1u << 31) / 79,
(1u << 31) / 80, (1u << 31) / 81, (1u << 31) / 82, (1u << 31) / 83,
(1u << 31) / 84, (1u << 31) / 85, (1u << 31) / 86, (1u << 31) / 87,
(1u << 31) / 88, (1u << 31) / 89, (1u << 31) / 90, (1u << 31) / 91,
(1u << 31) / 92, (1u << 31) / 93, (1u << 31) / 94, (1u << 31) / 95,
(1u << 31) / 96, (1u << 31) / 97, (1u << 31) / 98, (1u << 31) / 99,
(1u << 31) / 100, (1u << 31) / 101, (1u << 31) / 102, (1u << 31) / 103,
(1u << 31) / 104, (1u << 31) / 105, (1u << 31) / 106, (1u << 31) / 107,
(1u << 31) / 108, (1u << 31) / 109, (1u << 31) / 110, (1u << 31) / 111,
(1u << 31) / 112, (1u << 31) / 113, (1u << 31) / 114, (1u << 31) / 115,
(1u << 31) / 116, (1u << 31) / 117, (1u << 31) / 118, (1u << 31) / 119,
(1u << 31) / 120, (1u << 31) / 121, (1u << 31) / 122, (1u << 31) / 123,
(1u << 31) / 124, (1u << 31) / 125, (1u << 31) / 126, (1u << 31) / 127,
(1u << 31) / 128, (1u << 31) / 129, (1u << 31) / 130, (1u << 31) / 131,
(1u << 31) / 132, (1u << 31) / 133, (1u << 31) / 134, (1u << 31) / 135,
(1u << 31) / 136, (1u << 31) / 137, (1u << 31) / 138, (1u << 31) / 139,
(1u << 31) / 140, (1u << 31) / 141, (1u << 31) / 142, (1u << 31) / 143,
(1u << 31) / 144, (1u << 31) / 145, (1u << 31) / 146, (1u << 31) / 147,
(1u << 31) / 148, (1u << 31) / 149, (1u << 31) / 150, (1u << 31) / 151,
(1u << 31) / 152, (1u << 31) / 153, (1u << 31) / 154, (1u << 31) / 155,
(1u << 31) / 156, (1u << 31) / 157, (1u << 31) / 158, (1u << 31) / 159,
(1u << 31) / 160, (1u << 31) / 161, (1u << 31) / 162, (1u << 31) / 163,
(1u << 31) / 164, (1u << 31) / 165, (1u << 31) / 166, (1u << 31) / 167,
(1u << 31) / 168, (1u << 31) / 169, (1u << 31) / 170, (1u << 31) / 171,
(1u << 31) / 172, (1u << 31) / 173, (1u << 31) / 174, (1u << 31) / 175,
(1u << 31) / 176, (1u << 31) / 177, (1u << 31) / 178, (1u << 31) / 179,
(1u << 31) / 180, (1u << 31) / 181, (1u << 31) / 182, (1u << 31) / 183,
(1u << 31) / 184, (1u << 31) / 185, (1u << 31) / 186, (1u << 31) / 187,
(1u << 31) / 188, (1u << 31) / 189, (1u << 31) / 190, (1u << 31) / 191,
(1u << 31) / 192, (1u << 31) / 193, (1u << 31) / 194, (1u << 31) / 195,
(1u << 31) / 196, (1u << 31) / 197, (1u << 31) / 198, (1u << 31) / 199,
(1u << 31) / 200, (1u << 31) / 201, (1u << 31) / 202, (1u << 31) / 203,
(1u << 31) / 204, (1u << 31) / 205, (1u << 31) / 206, (1u << 31) / 207,
(1u << 31) / 208, (1u << 31) / 209, (1u << 31) / 210, (1u << 31) / 211,
(1u << 31) / 212, (1u << 31) / 213, (1u << 31) / 214, (1u << 31) / 215,
(1u << 31) / 216, (1u << 31) / 217, (1u << 31) / 218, (1u << 31) / 219,
(1u << 31) / 220, (1u << 31) / 221, (1u << 31) / 222, (1u << 31) / 223,
(1u << 31) / 224, (1u << 31) / 225, (1u << 31) / 226, (1u << 31) / 227,
(1u << 31) / 228, (1u << 31) / 229, (1u << 31) / 230, (1u << 31) / 231,
(1u << 31) / 232, (1u << 31) / 233, (1u << 31) / 234, (1u << 31) / 235,
(1u << 31) / 236, (1u << 31) / 237, (1u << 31) / 238, (1u << 31) / 239,
(1u << 31) / 240, (1u << 31) / 241, (1u << 31) / 242, (1u << 31) / 243,
(1u << 31) / 244, (1u << 31) / 245, (1u << 31) / 246, (1u << 31) / 247,
(1u << 31) / 248, (1u << 31) / 249, (1u << 31) / 250, (1u << 31) / 251,
(1u << 31) / 252, (1u << 31) / 253, (1u << 31) / 254, (1u << 31) / 255
};

/* *********************************************************** */

const mlib_s64 mlib_ImageArith_DivShift_U8[] = {
(1u << 24),
(1u << 16) / 1, (1u << 16) / 2, (1u << 16) / 3,
(1u << 16) / 4, (1u << 16) / 5, (1u << 16) / 6, (1u << 16) / 7,
(1u << 16) / 8, (1u << 16) / 9, (1u << 16) / 10, (1u << 16) / 11,
(1u << 16) / 12, (1u << 16) / 13, (1u << 16) / 14, (1u << 16) / 15,
(1u << 16) / 16, (1u << 16) / 17, (1u << 16) / 18, (1u << 16) / 19,
(1u << 16) / 20, (1u << 16) / 21, (1u << 16) / 22, (1u << 16) / 23,
(1u << 16) / 24, (1u << 16) / 25, (1u << 16) / 26, (1u << 16) / 27,
(1u << 16) / 28, (1u << 16) / 29, (1u << 16) / 30, (1u << 16) / 31,
(1u << 16) / 32, (1u << 16) / 33, (1u << 16) / 34, (1u << 16) / 35,
(1u << 16) / 36, (1u << 16) / 37, (1u << 16) / 38, (1u << 16) / 39,
(1u << 16) / 40, (1u << 16) / 41, (1u << 16) / 42, (1u << 16) / 43,
(1u << 16) / 44, (1u << 16) / 45, (1u << 16) / 46, (1u << 16) / 47,
(1u << 16) / 48, (1u << 16) / 49, (1u << 16) / 50, (1u << 16) / 51,
(1u << 16) / 52, (1u << 16) / 53, (1u << 16) / 54, (1u << 16) / 55,
(1u << 16) / 56, (1u << 16) / 57, (1u << 16) / 58, (1u << 16) / 59,
(1u << 16) / 60, (1u << 16) / 61, (1u << 16) / 62, (1u << 16) / 63,
(1u << 16) / 64, (1u << 16) / 65, (1u << 16) / 66, (1u << 16) / 67,
(1u << 16) / 68, (1u << 16) / 69, (1u << 16) / 70, (1u << 16) / 71,
(1u << 16) / 72, (1u << 16) / 73, (1u << 16) / 74, (1u << 16) / 75,
(1u << 16) / 76, (1u << 16) / 77, (1u << 16) / 78, (1u << 16) / 79,
(1u << 16) / 80, (1u << 16) / 81, (1u << 16) / 82, (1u << 16) / 83,
(1u << 16) / 84, (1u << 16) / 85, (1u << 16) / 86, (1u << 16) / 87,
(1u << 16) / 88, (1u << 16) / 89, (1u << 16) / 90, (1u << 16) / 91,
(1u << 16) / 92, (1u << 16) / 93, (1u << 16) / 94, (1u << 16) / 95,
(1u << 16) / 96, (1u << 16) / 97, (1u << 16) / 98, (1u << 16) / 99,
(1u << 16) / 100, (1u << 16) / 101, (1u << 16) / 102, (1u << 16) / 103,
(1u << 16) / 104, (1u << 16) / 105, (1u << 16) / 106, (1u << 16) / 107,
(1u << 16) / 108, (1u << 16) / 109, (1u << 16) / 110, (1u << 16) / 111,
(1u << 16) / 112, (1u << 16) / 113, (1u << 16) / 114, (1u << 16) / 115,
(1u << 16) / 116, (1u << 16) / 117, (1u << 16) / 118, (1u << 16) / 119,
(1u << 16) / 120, (1u << 16) / 121, (1u << 16) / 122, (1u << 16) / 123,
(1u << 16) / 124, (1u << 16) / 125, (1u << 16) / 126, (1u << 16) / 127,
(1u << 16) / 128, (1u << 16) / 129, (1u << 16) / 130, (1u << 16) / 131,
(1u << 16) / 132, (1u << 16) / 133, (1u << 16) / 134, (1u << 16) / 135,
(1u << 16) / 136, (1u << 16) / 137, (1u << 16) / 138, (1u << 16) / 139,
(1u << 16) / 140, (1u << 16) / 141, (1u << 16) / 142, (1u << 16) / 143,
(1u << 16) / 144, (1u << 16) / 145, (1u << 16) / 146, (1u << 16) / 147,
(1u << 16) / 148, (1u << 16) / 149, (1u << 16) / 150, (1u << 16) / 151,
(1u << 16) / 152, (1u << 16) / 153, (1u << 16) / 154, (1u << 16) / 155,
(1u << 16) / 156, (1u << 16) / 157, (1u << 16) / 158, (1u << 16) / 159,
(1u << 16) / 160, (1u << 16) / 161, (1u << 16) / 162, (1u << 16) / 163,
(1u << 16) / 164, (1u << 16) / 165, (1u << 16) / 166, (1u << 16) / 167,
(1u << 16) / 168, (1u << 16) / 169, (1u << 16) / 170, (1u << 16) / 171,
(1u << 16) / 172, (1u << 16) / 173, (1u << 16) / 174, (1u << 16) / 175,
(1u << 16) / 176, (1u << 16) / 177, (1u << 16) / 178, (1u << 16) / 179,
(1u << 16) / 180, (1u << 16) / 181, (1u << 16) / 182, (1u << 16) / 183,
(1u << 16) / 184, (1u << 16) / 185, (1u << 16) / 186, (1u << 16) / 187,
(1u << 16) / 188, (1u << 16) / 189, (1u << 16) / 190, (1u << 16) / 191,
(1u << 16) / 192, (1u << 16) / 193, (1u << 16) / 194, (1u << 16) / 195,
(1u << 16) / 196, (1u << 16) / 197, (1u << 16) / 198, (1u << 16) / 199,
(1u << 16) / 200, (1u << 16) / 201, (1u << 16) / 202, (1u << 16) / 203,
(1u << 16) / 204, (1u << 16) / 205, (1u << 16) / 206, (1u << 16) / 207,
(1u << 16) / 208, (1u << 16) / 209, (1u << 16) / 210, (1u << 16) / 211,
(1u << 16) / 212, (1u << 16) / 213, (1u << 16) / 214, (1u << 16) / 215,
(1u << 16) / 216, (1u << 16) / 217, (1u << 16) / 218, (1u << 16) / 219,
(1u << 16) / 220, (1u << 16) / 221, (1u << 16) / 222, (1u << 16) / 223,
(1u << 16) / 224, (1u << 16) / 225, (1u << 16) / 226, (1u << 16) / 227,
(1u << 16) / 228, (1u << 16) / 229, (1u << 16) / 230, (1u << 16) / 231,
(1u << 16) / 232, (1u << 16) / 233, (1u << 16) / 234, (1u << 16) / 235,
(1u << 16) / 236, (1u << 16) / 237, (1u << 16) / 238, (1u << 16) / 239,
(1u << 16) / 240, (1u << 16) / 241, (1u << 16) / 242, (1u << 16) / 243,
(1u << 16) / 244, (1u << 16) / 245, (1u << 16) / 246, (1u << 16) / 247,
(1u << 16) / 248, (1u << 16) / 249, (1u << 16) / 250, (1u << 16) / 251,
(1u << 16) / 252, (1u << 16) / 253, (1u << 16) / 254, (1u << 16) / 255
};

/* *********************************************************** */

const mlib_s64 mlib_ImageArith_ConstDiv_U8[] = {
549755813888,
(1u << 16) / 1, (1u << 16) / 2,
(1u << 16) / 3, (1u << 16) / 4,
(1u << 16) / 5, (1u << 16) / 6,
(1u << 16) / 7, (1u << 16) / 8,
(1u << 16) / 9, (1u << 16) / 10,
(1u << 16) / 11, (1u << 16) / 12,
(1u << 16) / 13, (1u << 16) / 14,
(1u << 16) / 15, (1u << 16) / 16,
(1u << 16) / 17, (1u << 16) / 18,
(1u << 16) / 19, (1u << 16) / 20,
(1u << 16) / 21, (1u << 16) / 22,
(1u << 16) / 23, (1u << 16) / 24,
(1u << 16) / 25, (1u << 16) / 26,
(1u << 16) / 27, (1u << 16) / 28,
(1u << 16) / 29, (1u << 16) / 30,
(1u << 16) / 31, (1u << 16) / 32,
(1u << 16) / 33, (1u << 16) / 34,
(1u << 16) / 35, (1u << 16) / 36,
(1u << 16) / 37, (1u << 16) / 38,
(1u << 16) / 39, (1u << 16) / 40,
(1u << 16) / 41, (1u << 16) / 42,
(1u << 16) / 43, (1u << 16) / 44,
(1u << 16) / 45, (1u << 16) / 46,
(1u << 16) / 47, (1u << 16) / 48,
(1u << 16) / 49, (1u << 16) / 50,
(1u << 16) / 51, (1u << 16) / 52,
(1u << 16) / 53, (1u << 16) / 54,
(1u << 16) / 55, (1u << 16) / 56,
(1u << 16) / 57, (1u << 16) / 58,
(1u << 16) / 59, (1u << 16) / 60,
(1u << 16) / 61, (1u << 16) / 62,
(1u << 16) / 63, (1u << 16) / 64,
(1u << 16) / 65, (1u << 16) / 66,
(1u << 16) / 67, (1u << 16) / 68,
(1u << 16) / 69, (1u << 16) / 70,
(1u << 16) / 71, (1u << 16) / 72,
(1u << 16) / 73, (1u << 16) / 74,
(1u << 16) / 75, (1u << 16) / 76,
(1u << 16) / 77, (1u << 16) / 78,
(1u << 16) / 79, (1u << 16) / 80,
(1u << 16) / 81, (1u << 16) / 82,
(1u << 16) / 83, (1u << 16) / 84,
(1u << 16) / 85, (1u << 16) / 86,
(1u << 16) / 87, (1u << 16) / 88,
(1u << 16) / 89, (1u << 16) / 90,
(1u << 16) / 91, (1u << 16) / 92,
(1u << 16) / 93, (1u << 16) / 94,
(1u << 16) / 95, (1u << 16) / 96,
(1u << 16) / 97, (1u << 16) / 98,
(1u << 16) / 99, (1u << 16) / 100,
(1u << 16) / 101, (1u << 16) / 102,
(1u << 16) / 103, (1u << 16) / 104,
(1u << 16) / 105, (1u << 16) / 106,
(1u << 16) / 107, (1u << 16) / 108,
(1u << 16) / 109, (1u << 16) / 110,
(1u << 16) / 111, (1u << 16) / 112,
(1u << 16) / 113, (1u << 16) / 114,
(1u << 16) / 115, (1u << 16) / 116,
(1u << 16) / 117, (1u << 16) / 118,
(1u << 16) / 119, (1u << 16) / 120,
(1u << 16) / 121, (1u << 16) / 122,
(1u << 16) / 123, (1u << 16) / 124,
(1u << 16) / 125, (1u << 16) / 126,
(1u << 16) / 127, (1u << 16) / 128,
(1u << 16) / 129, (1u << 16) / 130,
(1u << 16) / 131, (1u << 16) / 132,
(1u << 16) / 133, (1u << 16) / 134,
(1u << 16) / 135, (1u << 16) / 136,
(1u << 16) / 137, (1u << 16) / 138,
(1u << 16) / 139, (1u << 16) / 140,
(1u << 16) / 141, (1u << 16) / 142,
(1u << 16) / 143, (1u << 16) / 144,
(1u << 16) / 145, (1u << 16) / 146,
(1u << 16) / 147, (1u << 16) / 148,
(1u << 16) / 149, (1u << 16) / 150,
(1u << 16) / 151, (1u << 16) / 152,
(1u << 16) / 153, (1u << 16) / 154,
(1u << 16) / 155, (1u << 16) / 156,
(1u << 16) / 157, (1u << 16) / 158,
(1u << 16) / 159, (1u << 16) / 160,
(1u << 16) / 161, (1u << 16) / 162,
(1u << 16) / 163, (1u << 16) / 164,
(1u << 16) / 165, (1u << 16) / 166,
(1u << 16) / 167, (1u << 16) / 168,
(1u << 16) / 169, (1u << 16) / 170,
(1u << 16) / 171, (1u << 16) / 172,
(1u << 16) / 173, (1u << 16) / 174,
(1u << 16) / 175, (1u << 16) / 176,
(1u << 16) / 177, (1u << 16) / 178,
(1u << 16) / 179, (1u << 16) / 180,
(1u << 16) / 181, (1u << 16) / 182,
(1u << 16) / 183, (1u << 16) / 184,
(1u << 16) / 185, (1u << 16) / 186,
(1u << 16) / 187, (1u << 16) / 188,
(1u << 16) / 189, (1u << 16) / 190,
(1u << 16) / 191, (1u << 16) / 192,
(1u << 16) / 193, (1u << 16) / 194,
(1u << 16) / 195, (1u << 16) / 196,
(1u << 16) / 197, (1u << 16) / 198,
(1u << 16) / 199, (1u << 16) / 200,
(1u << 16) / 201, (1u << 16) / 202,
(1u << 16) / 203, (1u << 16) / 204,
(1u << 16) / 205, (1u << 16) / 206,
(1u << 16) / 207, (1u << 16) / 208,
(1u << 16) / 209, (1u << 16) / 210,
(1u << 16) / 211, (1u << 16) / 212,
(1u << 16) / 213, (1u << 16) / 214,
(1u << 16) / 215, (1u << 16) / 216,
(1u << 16) / 217, (1u << 16) / 218,
(1u << 16) / 219, (1u << 16) / 220,
(1u << 16) / 221, (1u << 16) / 222,
(1u << 16) / 223, (1u << 16) / 224,
(1u << 16) / 225, (1u << 16) / 226,
(1u << 16) / 227, (1u << 16) / 228,
(1u << 16) / 229, (1u << 16) / 230,
(1u << 16) / 231, (1u << 16) / 232,
(1u << 16) / 233, (1u << 16) / 234,
(1u << 16) / 235, (1u << 16) / 236,
(1u << 16) / 237, (1u << 16) / 238,
(1u << 16) / 239, (1u << 16) / 240,
(1u << 16) / 241, (1u << 16) / 242,
(1u << 16) / 243, (1u << 16) / 244,
(1u << 16) / 245, (1u << 16) / 246,
(1u << 16) / 247, (1u << 16) / 248,
(1u << 16) / 249, (1u << 16) / 250,
(1u << 16) / 251, (1u << 16) / 252,
(1u << 16) / 253, (1u << 16) / 254,
(1u << 16) / 255
};