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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageZoomTranslateBlend.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslateBlend(), internal pixels.
 *
 */

#include <mlib_s_ImageZoom.h>

/* *********************************************************** */

typedef union
{
	struct
	{
		unsigned int i[4];
	} int4;
	__m128i m128i;
} mlib_m128i_union;

extern mlib_m128i_union mlib_emask_m128i[];

/* *********************************************************** */

typedef union
{
	struct
	{
		mlib_u16 a, b, c, d;
	} s16x4;
	mlib_d64 d64;
} mlib_d64_s16x4;

const mlib_d64_s16x4 mlib_m_tbl_255DivAlpha[] = {
	{0x0000, 0x0000, 0x0000, 0x0000},
	{0xff00, 0xff00, 0xff00, 0xff00},
	{0x7f80, 0x7f80, 0x7f80, 0x7f80},
	{0x5500, 0x5500, 0x5500, 0x5500},
	{0x3fc0, 0x3fc0, 0x3fc0, 0x3fc0},
	{0x3300, 0x3300, 0x3300, 0x3300},
	{0x2a80, 0x2a80, 0x2a80, 0x2a80},
	{0x246e, 0x246e, 0x246e, 0x246e},
	{0x1fe0, 0x1fe0, 0x1fe0, 0x1fe0},
	{0x1c55, 0x1c55, 0x1c55, 0x1c55},
	{0x1980, 0x1980, 0x1980, 0x1980},
	{0x172f, 0x172f, 0x172f, 0x172f},
	{0x1540, 0x1540, 0x1540, 0x1540},
	{0x139e, 0x139e, 0x139e, 0x139e},
	{0x1237, 0x1237, 0x1237, 0x1237},
	{0x1100, 0x1100, 0x1100, 0x1100},
	{0x0ff0, 0x0ff0, 0x0ff0, 0x0ff0},
	{0x0f00, 0x0f00, 0x0f00, 0x0f00},
	{0x0e2b, 0x0e2b, 0x0e2b, 0x0e2b},
	{0x0d6c, 0x0d6c, 0x0d6c, 0x0d6c},
	{0x0cc0, 0x0cc0, 0x0cc0, 0x0cc0},
	{0x0c25, 0x0c25, 0x0c25, 0x0c25},
	{0x0b97, 0x0b97, 0x0b97, 0x0b97},
	{0x0b16, 0x0b16, 0x0b16, 0x0b16},
	{0x0aa0, 0x0aa0, 0x0aa0, 0x0aa0},
	{0x0a33, 0x0a33, 0x0a33, 0x0a33},
	{0x09cf, 0x09cf, 0x09cf, 0x09cf},
	{0x0972, 0x0972, 0x0972, 0x0972},
	{0x091b, 0x091b, 0x091b, 0x091b},
	{0x08cb, 0x08cb, 0x08cb, 0x08cb},
	{0x0880, 0x0880, 0x0880, 0x0880},
	{0x083a, 0x083a, 0x083a, 0x083a},
	{0x07f8, 0x07f8, 0x07f8, 0x07f8},
	{0x07ba, 0x07ba, 0x07ba, 0x07ba},
	{0x0780, 0x0780, 0x0780, 0x0780},
	{0x0749, 0x0749, 0x0749, 0x0749},
	{0x0715, 0x0715, 0x0715, 0x0715},
	{0x06e4, 0x06e4, 0x06e4, 0x06e4},
	{0x06b6, 0x06b6, 0x06b6, 0x06b6},
	{0x068a, 0x068a, 0x068a, 0x068a},
	{0x0660, 0x0660, 0x0660, 0x0660},
	{0x0638, 0x0638, 0x0638, 0x0638},
	{0x0612, 0x0612, 0x0612, 0x0612},
	{0x05ee, 0x05ee, 0x05ee, 0x05ee},
	{0x05cc, 0x05cc, 0x05cc, 0x05cc},
	{0x05ab, 0x05ab, 0x05ab, 0x05ab},
	{0x058b, 0x058b, 0x058b, 0x058b},
	{0x056d, 0x056d, 0x056d, 0x056d},
	{0x0550, 0x0550, 0x0550, 0x0550},
	{0x0534, 0x0534, 0x0534, 0x0534},
	{0x051a, 0x051a, 0x051a, 0x051a},
	{0x0500, 0x0500, 0x0500, 0x0500},
	{0x04e7, 0x04e7, 0x04e7, 0x04e7},
	{0x04d0, 0x04d0, 0x04d0, 0x04d0},
	{0x04b9, 0x04b9, 0x04b9, 0x04b9},
	{0x04a3, 0x04a3, 0x04a3, 0x04a3},
	{0x048e, 0x048e, 0x048e, 0x048e},
	{0x0479, 0x0479, 0x0479, 0x0479},
	{0x0466, 0x0466, 0x0466, 0x0466},
	{0x0452, 0x0452, 0x0452, 0x0452},
	{0x0440, 0x0440, 0x0440, 0x0440},
	{0x042e, 0x042e, 0x042e, 0x042e},
	{0x041d, 0x041d, 0x041d, 0x041d},
	{0x040c, 0x040c, 0x040c, 0x040c},
	{0x03fc, 0x03fc, 0x03fc, 0x03fc},
	{0x03ec, 0x03ec, 0x03ec, 0x03ec},
	{0x03dd, 0x03dd, 0x03dd, 0x03dd},
	{0x03ce, 0x03ce, 0x03ce, 0x03ce},
	{0x03c0, 0x03c0, 0x03c0, 0x03c0},
	{0x03b2, 0x03b2, 0x03b2, 0x03b2},
	{0x03a5, 0x03a5, 0x03a5, 0x03a5},
	{0x0397, 0x0397, 0x0397, 0x0397},
	{0x038b, 0x038b, 0x038b, 0x038b},
	{0x037e, 0x037e, 0x037e, 0x037e},
	{0x0372, 0x0372, 0x0372, 0x0372},
	{0x0366, 0x0366, 0x0366, 0x0366},
	{0x035b, 0x035b, 0x035b, 0x035b},
	{0x0350, 0x0350, 0x0350, 0x0350},
	{0x0345, 0x0345, 0x0345, 0x0345},
	{0x033a, 0x033a, 0x033a, 0x033a},
	{0x0330, 0x0330, 0x0330, 0x0330},
	{0x0326, 0x0326, 0x0326, 0x0326},
	{0x031c, 0x031c, 0x031c, 0x031c},
	{0x0313, 0x0313, 0x0313, 0x0313},
	{0x0309, 0x0309, 0x0309, 0x0309},
	{0x0300, 0x0300, 0x0300, 0x0300},
	{0x02f7, 0x02f7, 0x02f7, 0x02f7},
	{0x02ee, 0x02ee, 0x02ee, 0x02ee},
	{0x02e6, 0x02e6, 0x02e6, 0x02e6},
	{0x02dd, 0x02dd, 0x02dd, 0x02dd},
	{0x02d5, 0x02d5, 0x02d5, 0x02d5},
	{0x02cd, 0x02cd, 0x02cd, 0x02cd},
	{0x02c6, 0x02c6, 0x02c6, 0x02c6},
	{0x02be, 0x02be, 0x02be, 0x02be},
	{0x02b6, 0x02b6, 0x02b6, 0x02b6},
	{0x02af, 0x02af, 0x02af, 0x02af},
	{0x02a8, 0x02a8, 0x02a8, 0x02a8},
	{0x02a1, 0x02a1, 0x02a1, 0x02a1},
	{0x029a, 0x029a, 0x029a, 0x029a},
	{0x0293, 0x0293, 0x0293, 0x0293},
	{0x028d, 0x028d, 0x028d, 0x028d},
	{0x0286, 0x0286, 0x0286, 0x0286},
	{0x0280, 0x0280, 0x0280, 0x0280},
	{0x027a, 0x027a, 0x027a, 0x027a},
	{0x0274, 0x0274, 0x0274, 0x0274},
	{0x026e, 0x026e, 0x026e, 0x026e},
	{0x0268, 0x0268, 0x0268, 0x0268},
	{0x0262, 0x0262, 0x0262, 0x0262},
	{0x025c, 0x025c, 0x025c, 0x025c},
	{0x0257, 0x0257, 0x0257, 0x0257},
	{0x0251, 0x0251, 0x0251, 0x0251},
	{0x024c, 0x024c, 0x024c, 0x024c},
	{0x0247, 0x0247, 0x0247, 0x0247},
	{0x0242, 0x0242, 0x0242, 0x0242},
	{0x023d, 0x023d, 0x023d, 0x023d},
	{0x0238, 0x0238, 0x0238, 0x0238},
	{0x0233, 0x0233, 0x0233, 0x0233},
	{0x022e, 0x022e, 0x022e, 0x022e},
	{0x0229, 0x0229, 0x0229, 0x0229},
	{0x0225, 0x0225, 0x0225, 0x0225},
	{0x0220, 0x0220, 0x0220, 0x0220},
	{0x021c, 0x021c, 0x021c, 0x021c},
	{0x0217, 0x0217, 0x0217, 0x0217},
	{0x0213, 0x0213, 0x0213, 0x0213},
	{0x020e, 0x020e, 0x020e, 0x020e},
	{0x020a, 0x020a, 0x020a, 0x020a},
	{0x0206, 0x0206, 0x0206, 0x0206},
	{0x0202, 0x0202, 0x0202, 0x0202},
	{0x01fe, 0x01fe, 0x01fe, 0x01fe},
	{0x01fa, 0x01fa, 0x01fa, 0x01fa},
	{0x01f6, 0x01f6, 0x01f6, 0x01f6},
	{0x01f2, 0x01f2, 0x01f2, 0x01f2},
	{0x01ef, 0x01ef, 0x01ef, 0x01ef},
	{0x01eb, 0x01eb, 0x01eb, 0x01eb},
	{0x01e7, 0x01e7, 0x01e7, 0x01e7},
	{0x01e4, 0x01e4, 0x01e4, 0x01e4},
	{0x01e0, 0x01e0, 0x01e0, 0x01e0},
	{0x01dc, 0x01dc, 0x01dc, 0x01dc},
	{0x01d9, 0x01d9, 0x01d9, 0x01d9},
	{0x01d6, 0x01d6, 0x01d6, 0x01d6},
	{0x01d2, 0x01d2, 0x01d2, 0x01d2},
	{0x01cf, 0x01cf, 0x01cf, 0x01cf},
	{0x01cc, 0x01cc, 0x01cc, 0x01cc},
	{0x01c9, 0x01c9, 0x01c9, 0x01c9},
	{0x01c5, 0x01c5, 0x01c5, 0x01c5},
	{0x01c2, 0x01c2, 0x01c2, 0x01c2},
	{0x01bf, 0x01bf, 0x01bf, 0x01bf},
	{0x01bc, 0x01bc, 0x01bc, 0x01bc},
	{0x01b9, 0x01b9, 0x01b9, 0x01b9},
	{0x01b6, 0x01b6, 0x01b6, 0x01b6},
	{0x01b3, 0x01b3, 0x01b3, 0x01b3},
	{0x01b0, 0x01b0, 0x01b0, 0x01b0},
	{0x01ad, 0x01ad, 0x01ad, 0x01ad},
	{0x01ab, 0x01ab, 0x01ab, 0x01ab},
	{0x01a8, 0x01a8, 0x01a8, 0x01a8},
	{0x01a5, 0x01a5, 0x01a5, 0x01a5},
	{0x01a2, 0x01a2, 0x01a2, 0x01a2},
	{0x01a0, 0x01a0, 0x01a0, 0x01a0},
	{0x019d, 0x019d, 0x019d, 0x019d},
	{0x019b, 0x019b, 0x019b, 0x019b},
	{0x0198, 0x0198, 0x0198, 0x0198},
	{0x0195, 0x0195, 0x0195, 0x0195},
	{0x0193, 0x0193, 0x0193, 0x0193},
	{0x0190, 0x0190, 0x0190, 0x0190},
	{0x018e, 0x018e, 0x018e, 0x018e},
	{0x018c, 0x018c, 0x018c, 0x018c},
	{0x0189, 0x0189, 0x0189, 0x0189},
	{0x0187, 0x0187, 0x0187, 0x0187},
	{0x0185, 0x0185, 0x0185, 0x0185},
	{0x0182, 0x0182, 0x0182, 0x0182},
	{0x0180, 0x0180, 0x0180, 0x0180},
	{0x017e, 0x017e, 0x017e, 0x017e},
	{0x017c, 0x017c, 0x017c, 0x017c},
	{0x0179, 0x0179, 0x0179, 0x0179},
	{0x0177, 0x0177, 0x0177, 0x0177},
	{0x0175, 0x0175, 0x0175, 0x0175},
	{0x0173, 0x0173, 0x0173, 0x0173},
	{0x0171, 0x0171, 0x0171, 0x0171},
	{0x016f, 0x016f, 0x016f, 0x016f},
	{0x016d, 0x016d, 0x016d, 0x016d},
	{0x016b, 0x016b, 0x016b, 0x016b},
	{0x0169, 0x0169, 0x0169, 0x0169},
	{0x0167, 0x0167, 0x0167, 0x0167},
	{0x0165, 0x0165, 0x0165, 0x0165},
	{0x0163, 0x0163, 0x0163, 0x0163},
	{0x0161, 0x0161, 0x0161, 0x0161},
	{0x015f, 0x015f, 0x015f, 0x015f},
	{0x015d, 0x015d, 0x015d, 0x015d},
	{0x015b, 0x015b, 0x015b, 0x015b},
	{0x0159, 0x0159, 0x0159, 0x0159},
	{0x0158, 0x0158, 0x0158, 0x0158},
	{0x0156, 0x0156, 0x0156, 0x0156},
	{0x0154, 0x0154, 0x0154, 0x0154},
	{0x0152, 0x0152, 0x0152, 0x0152},
	{0x0150, 0x0150, 0x0150, 0x0150},
	{0x014f, 0x014f, 0x014f, 0x014f},
	{0x014d, 0x014d, 0x014d, 0x014d},
	{0x014b, 0x014b, 0x014b, 0x014b},
	{0x014a, 0x014a, 0x014a, 0x014a},
	{0x0148, 0x0148, 0x0148, 0x0148},
	{0x0146, 0x0146, 0x0146, 0x0146},
	{0x0145, 0x0145, 0x0145, 0x0145},
	{0x0143, 0x0143, 0x0143, 0x0143},
	{0x0142, 0x0142, 0x0142, 0x0142},
	{0x0140, 0x0140, 0x0140, 0x0140},
	{0x013e, 0x013e, 0x013e, 0x013e},
	{0x013d, 0x013d, 0x013d, 0x013d},
	{0x013b, 0x013b, 0x013b, 0x013b},
	{0x013a, 0x013a, 0x013a, 0x013a},
	{0x0138, 0x0138, 0x0138, 0x0138},
	{0x0137, 0x0137, 0x0137, 0x0137},
	{0x0135, 0x0135, 0x0135, 0x0135},
	{0x0134, 0x0134, 0x0134, 0x0134},
	{0x0132, 0x0132, 0x0132, 0x0132},
	{0x0131, 0x0131, 0x0131, 0x0131},
	{0x0130, 0x0130, 0x0130, 0x0130},
	{0x012e, 0x012e, 0x012e, 0x012e},
	{0x012d, 0x012d, 0x012d, 0x012d},
	{0x012b, 0x012b, 0x012b, 0x012b},
	{0x012a, 0x012a, 0x012a, 0x012a},
	{0x0129, 0x0129, 0x0129, 0x0129},
	{0x0127, 0x0127, 0x0127, 0x0127},
	{0x0126, 0x0126, 0x0126, 0x0126},
	{0x0125, 0x0125, 0x0125, 0x0125},
	{0x0123, 0x0123, 0x0123, 0x0123},
	{0x0122, 0x0122, 0x0122, 0x0122},
	{0x0121, 0x0121, 0x0121, 0x0121},
	{0x0120, 0x0120, 0x0120, 0x0120},
	{0x011e, 0x011e, 0x011e, 0x011e},
	{0x011d, 0x011d, 0x011d, 0x011d},
	{0x011c, 0x011c, 0x011c, 0x011c},
	{0x011b, 0x011b, 0x011b, 0x011b},
	{0x0119, 0x0119, 0x0119, 0x0119},
	{0x0118, 0x0118, 0x0118, 0x0118},
	{0x0117, 0x0117, 0x0117, 0x0117},
	{0x0116, 0x0116, 0x0116, 0x0116},
	{0x0115, 0x0115, 0x0115, 0x0115},
	{0x0113, 0x0113, 0x0113, 0x0113},
	{0x0112, 0x0112, 0x0112, 0x0112},
	{0x0111, 0x0111, 0x0111, 0x0111},
	{0x0110, 0x0110, 0x0110, 0x0110},
	{0x010f, 0x010f, 0x010f, 0x010f},
	{0x010e, 0x010e, 0x010e, 0x010e},
	{0x010d, 0x010d, 0x010d, 0x010d},
	{0x010c, 0x010c, 0x010c, 0x010c},
	{0x010a, 0x010a, 0x010a, 0x010a},
	{0x0109, 0x0109, 0x0109, 0x0109},
	{0x0108, 0x0108, 0x0108, 0x0108},
	{0x0107, 0x0107, 0x0107, 0x0107},
	{0x0106, 0x0106, 0x0106, 0x0106},
	{0x0105, 0x0105, 0x0105, 0x0105},
	{0x0104, 0x0104, 0x0104, 0x0104},
	{0x0103, 0x0103, 0x0103, 0x0103},
	{0x0102, 0x0102, 0x0102, 0x0102},
	{0x0101, 0x0101, 0x0101, 0x0101},
	{0x0100, 0x0100, 0x0100, 0x0100},
};

/* *********************************************************** */

#define	BUFF_SIZE	2048

/* *********************************************************** */

#define	BLEND43_SRC_OVER(bmask)                                        \
	dd = _mm_loadl_epi64((void *)((mlib_d64 *)buffd + i));         \
	d0 = _mm_unpacklo_epi8(mzero, dd);                             \
	s0 = buffz[i];                                                 \
	a0 = _mm_shufflelo_epi16(s0, bmask);                           \
	a0 = _mm_shufflehi_epi16(a0, bmask);                           \
	a0 = _mm_mulhi_epu16(a0, dalp);                                \
	a0 = _mm_sub_epi16(done, a0);                                  \
	d0 = _mm_adds_epu16(_mm_mulhi_epu16(dalp, s0),                 \
			    _mm_mulhi_epu16(a0, d0));                  \
	d0 = _mm_srli_epi16(d0, 7);                                    \
	dd = _mm_packus_epi16(d0, d0);                                 \
	_mm_storel_epi64((void *)((mlib_d64 *)buffd + i), dd)

/* *********************************************************** */

#define	GET_ALPHA_CHANNEL_0(dd)	_mm_and_si128(dd, amask)

#define	GET_ALPHA_CHANNEL_3(dd)	_mm_srli_epi32(dd, 24)

#define	GET_ALPHA_CHANNEL16_0(a0, a1)                           \
	_mm_and_si128(_mm_unpacklo_epi32(a0, a1), a16mask)

#define	GET_ALPHA_CHANNEL16_3(a0, a1)                           \
	_mm_srli_epi32(_mm_unpackhi_epi32(a0, a1), 16)

#define	PUT_ALPHA_CHANNEL_0(aa)	aa

#define	PUT_ALPHA_CHANNEL_3(aa)	_mm_slli_epi32(aa, 24)

#define	CONVERT_34_0(ss)	_mm_slli_epi64(ss, 16)

#define	CONVERT_34_3(ss)	ss

/* *********************************************************** */

#define	BLEND34_SRC_OVER(alpind)                                           \
	s0 = CONVERT_34_##alpind(buffz[2 * i]);                            \
	s1 = CONVERT_34_##alpind(buffz[2 * i + 1]);                        \
	dd = _mm_loadu_si128((__m128i *) tdp);                             \
	aa = GET_ALPHA_CHANNEL_##alpind(dd);                               \
	af = _mm_cvtepi32_ps(aa);                                          \
	w1 = _mm_mul_ps(w1s, af);                                          \
	w = _mm_add_ps(w0, w1);                                            \
	rw = _mm_div_ps(scale, w);                                         \
	w0r = _mm_mul_ps(w0, rw);                                          \
	w1r = _mm_mul_ps(w1, rw);                                          \
	aa = _mm_cvtps_epi32(w0r);                                         \
	rr = _mm_cvtps_epi32(w1r);                                         \
	wi = PUT_ALPHA_CHANNEL_##alpind(_mm_cvtps_epi32(w));               \
	a0 = _mm_unpacklo_epi16(aa, mzero);                                \
	a1 = _mm_unpackhi_epi16(aa, mzero);                                \
	r0 = _mm_unpacklo_epi16(rr, mzero);                                \
	r1 = _mm_unpackhi_epi16(rr, mzero);                                \
	d0 = _mm_unpacklo_epi8(mzero, dd);                                 \
	d1 = _mm_unpackhi_epi8(mzero, dd);                                 \
	a0 = _mm_shufflelo_epi16(a0, 0);                                   \
	a0 = _mm_shufflehi_epi16(a0, 0);                                   \
	r0 = _mm_shufflelo_epi16(r0, 0);                                   \
	r0 = _mm_shufflehi_epi16(r0, 0);                                   \
	d0 = _mm_adds_epu16(_mm_mulhi_epu16(a0, s0),                       \
			    _mm_mulhi_epu16(r0, d0));                      \
	a1 = _mm_shufflelo_epi16(a1, 0);                                   \
	a1 = _mm_shufflehi_epi16(a1, 0);                                   \
	r1 = _mm_shufflelo_epi16(r1, 0);                                   \
	r1 = _mm_shufflehi_epi16(r1, 0);                                   \
	d1 = _mm_adds_epu16(_mm_mulhi_epu16(a1, s1),                       \
			    _mm_mulhi_epu16(r1, d1));                      \
	d0 = _mm_srli_epi16(d0, 7);                                        \
	d1 = _mm_srli_epi16(d1, 7);                                        \
	dr = _mm_packus_epi16(d0, d1);                                     \
	dr = _mm_or_si128(wi, _mm_andnot_si128(amask, dr));

/* *********************************************************** */

/*
 * This macro is used in the cases of blending ARGB src with ARGB dst.
 *
 * For mlib_ImageZoomTranslateBlend(),
 *
 *                  a0 = (alpha * interp_a) / 0xff;
 *
 *                  int w0 = 0xff * a0;
 *                  int w1 = (0xff - a0) * dst_a
 *                  int w = w0 + w1;
 *
 *                  dst_r = (w0 * interp_r + w1 * dst_r) / w;
 *                  dst_g = (w0 * interp_g + w1 * dst_g) / w;
 *                  dst_b = (w0 * interp_b + w1 * dst_b) / w;
 *                  dst_a = w / 0xff;
 *
 * For mlib_ImageZoomTranslateTableBlend(),
 *
 *                  a0 = interp_a;
 *
 *                  int a1 = (0xff - a0) * dst_a / 0xff;
 *                  int w = a0 + a1;
 *
 *                  if (w != 0) {
 *                      dst_r = (0xff * interp_r + a1 * dst_r) / w;
 *                      dst_g = (0xff * interp_g + a1 * dst_g) / w;
 *                      dst_b = (0xff * interp_b + a1 * dst_b) / w;
 *                      dst_a = w;
 *                  } else if (MLIB_BLEND_GTK_SRC_OVER) {
 *                      dst_r = 0;
 *                      dst_g = 0;
 *                      dst_b = 0;
 *                      dst_a = 0;
 *                  }
 *
 * It is implemented as follows, mainly for precision reasons:
 *
 *	dst_r = (alpha/w) * premultiplied_interp_r + (w1/w) * dst_r;
 *
 *	        r0, r1      s0, s1                   a0, a1   d0, d1
 */

#define	MASK_SRC_OVER

#define	MASK_SRC_OVER2                                             \
	dr = _mm_or_si128(dr, _mm_andnot_si128(zero_mask_i, dd))

#define	BLEND44(SRC_OVER2, alpind)                                      \
	s0 = buffz[2 * i];                                              \
	s1 = buffz[2 * i + 1];                                          \
	a0 = _mm_unpacklo_epi32(s0, s1);                                \
	a1 = _mm_unpackhi_epi32(s0, s1);                                \
	ss = GET_ALPHA_CHANNEL16_##alpind(a0, a1);                      \
	dd = _mm_loadu_si128((__m128i *) tdp);                          \
	aa = GET_ALPHA_CHANNEL_##alpind(dd);                            \
	af = _mm_cvtepi32_ps(aa);                                       \
	sf = _mm_cvtepi32_ps(ss);                                       \
	w0 = _mm_mul_ps(dalp, sf);                                      \
	w1 = _mm_mul_ps(w0, div255);                                    \
	w1 = _mm_sub_ps(fone, w1);                                      \
	w1 = _mm_mul_ps(w1, af);                                        \
	w = _mm_add_ps(w0, w1);                                         \
	zero_mask = _mm_cmpge_ps(w, fone);                              \
	rw = _mm_div_ps(scale, w);                                      \
	w1 = _mm_mul_ps(w1, rw);                                        \
	w1 = _mm_add_ps(w1, f_rnd);                                     \
	w0 = _mm_mul_ps(alpha, rw);                                     \
	w0 = _mm_add_ps(w0, f_rnd);                                     \
	aa = _mm_cvttps_epi32(w1);                                      \
	rr = _mm_cvttps_epi32(w0);                                      \
	wi = PUT_ALPHA_CHANNEL_##alpind(_mm_cvttps_epi32(w));           \
	a0 = _mm_unpacklo_epi16(aa, mzero);                             \
	a1 = _mm_unpackhi_epi16(aa, mzero);                             \
	r0 = _mm_unpacklo_epi16(rr, mzero);                             \
	r1 = _mm_unpackhi_epi16(rr, mzero);                             \
	d0 = _mm_unpacklo_epi8(mzero, dd);                              \
	d1 = _mm_unpackhi_epi8(mzero, dd);                              \
	a0 = _mm_shufflelo_epi16(a0, 0);                                \
	a0 = _mm_shufflehi_epi16(a0, 0);                                \
	r0 = _mm_shufflelo_epi16(r0, 0);                                \
	r0 = _mm_shufflehi_epi16(r0, 0);                                \
	d0 = _mm_adds_epu16(_mm_mulhi_epu16(r0, s0),                    \
			    _mm_mulhi_epu16(a0, d0));                   \
	a1 = _mm_shufflelo_epi16(a1, 0);                                \
	a1 = _mm_shufflehi_epi16(a1, 0);                                \
	r1 = _mm_shufflelo_epi16(r1, 0);                                \
	r1 = _mm_shufflehi_epi16(r1, 0);                                \
	d1 = _mm_adds_epu16(_mm_mulhi_epu16(r1, s1),                    \
			    _mm_mulhi_epu16(a1, d1));                   \
	dr = _mm_packus_epi16(d0, d1);                                  \
	dr = _mm_or_si128(wi, _mm_andnot_si128(amask, dr));             \
	zero_mask_i = *(__m128i *) & zero_mask;                         \
	dr = _mm_and_si128(dr, zero_mask_i);                            \
	MASK_##SRC_OVER2

/* *********************************************************** */

void
mlib_s_ImageBlendLine(
    mlib_work_image * param,
    mlib_u8 *dp,
    __m128i * buffz,
    __m128i * buffd)
{
	mlib_blend blend = param->blend;
	mlib_s32 chan_d = param->chan_d;
	mlib_s32 chan_s = param->channels;
	mlib_d64 alp = (param->alpha) * (1.0 / 255);
	mlib_s32 width = GetElemSubStruct(current, width);
	mlib_u8 *tdp = dp;
	mlib_s32 width2, y_step, next_step = 2;
	mlib_s32 alp_ind = param->alp_ind, mask255;
	__m128i aa, dalp, done;
	__m128i mzero, mask_7fff, mask_8000, amask, amask256, amaskffff;
	__m128i d_rnd;
	mlib_s32 i, j;

	if (!alp_ind) {
		d_rnd = _mm_set1_epi16(0x0080);

		tdp = (void *)dp;
		if (chan_d == 3)
			tdp = (void *)buffd;

		for (i = 0; i < width / 2; i++) {
			__m128i dd;

			dd = buffz[i];
			dd = _mm_adds_epu16(dd, d_rnd);
			dd = _mm_srli_epi16(dd, 8);
			dd = _mm_packus_epi16(dd, dd);
			_mm_storel_epi64((void *)(tdp + 8 * i), dd);
		}
		if (width & 1) {
			__m128i dd;

			dd = buffz[i];
			dd = _mm_adds_epu16(dd, d_rnd);
			dd = _mm_srli_epi16(dd, 8);
			dd = _mm_packus_epi16(dd, dd);
			*(mlib_s32 *)(tdp + 8 * i) = *(mlib_s32 *)&dd;
		}

		if (chan_d == 3) {
			mlib_s_ImageChannelExtract_U8_43L_D1((void *)buffd, dp,
			    width);
		}
		return;
	}

	width2 = (width + 1) / 2;

	mzero = _mm_setzero_si128();
	mask_7fff = _mm_set1_epi16(0x7FFF);
	mask_8000 = _mm_set1_epi16(0x8000);
	done = _mm_set1_epi16(1 << 15);
	if (alp_ind == -1) {
		mask255 = 0xFF;
		amask = _mm_setr_epi32(0xff00, 0, 0xff00, 0);
		amaskffff = _mm_setr_epi32(0xffff, 0, 0xffff, 0);
		amask256 = _mm_setr_epi32(0x0100, 0, 0x0100, 0);
	} else {
		mask255 = 0xFF000000;
		amask = _mm_setr_epi32(0, 0xff000000, 0, 0xff000000);
		amaskffff = _mm_setr_epi32(0, 0xffff0000, 0, 0xffff0000);
		amask256 = _mm_setr_epi32(0, 0x01000000, 0, 0x01000000);
	}
	dalp = _mm_set1_epi16((1 << 15) * alp + 0.5);

	if (chan_s == 3) {
		if (chan_d == 3) {
			mlib_d64 alp = (param->alpha) * (1.0 / 255);
			mlib_s32 ialp;
			mlib_u8 *pz;
			__m128i emask;
			__m128i dalp, ralp, ss, dd, s0, s1, d0, d1, dr;

			mlib_s_ImageChannelExtract_S16_43L_D1((void *)buffz,
			    (void *)buffd, width);

			ialp = alp * (1 << 15);
			dalp = _mm_set1_epi16(ialp);
			ralp = _mm_set1_epi16((1 << 15) - ialp);
			emask = mlib_emask_m128i[(3 * width) & 15].m128i;

			pz = (void *)buffd;
			tdp = dp;
			for (i = 0; i <= 3 * width - 16; i += 16) {
				s0 = _mm_load_si128((__m128i *) (pz + 2 * i));
				s1 = _mm_load_si128((__m128i *) (pz + 2 * i +
				    16));
				dd = _mm_loadu_si128((__m128i *) (tdp + i));
				d0 = _mm_unpacklo_epi8(mzero, dd);
				d1 = _mm_unpackhi_epi8(mzero, dd);
				d0 = _mm_add_epi16(_mm_mulhi_epu16(s0, dalp),
				    _mm_mulhi_epu16(d0, ralp));
				d1 = _mm_add_epi16(_mm_mulhi_epu16(s1, dalp),
				    _mm_mulhi_epu16(d1, ralp));
				d0 = _mm_srli_epi16(d0, 7);
				d1 = _mm_srli_epi16(d1, 7);
				dr = _mm_packus_epi16(d0, d1);
				_mm_storeu_si128((__m128i *) (tdp + i), dr);
			}

			if (i < 3 * width) {
				s0 = _mm_load_si128((__m128i *) (pz + 2 * i));
				s1 = _mm_load_si128((__m128i *) (pz + 2 * i +
				    16));
				dd = _mm_loadu_si128((__m128i *) (tdp + i));
				d0 = _mm_unpacklo_epi8(mzero, dd);
				d1 = _mm_unpackhi_epi8(mzero, dd);
				d0 = _mm_add_epi16(_mm_mulhi_epu16(s0, dalp),
				    _mm_mulhi_epu16(d0, ralp));
				d1 = _mm_add_epi16(_mm_mulhi_epu16(s1, dalp),
				    _mm_mulhi_epu16(d1, ralp));
				d0 = _mm_srli_epi16(d0, 7);
				d1 = _mm_srli_epi16(d1, 7);
				dr = _mm_packus_epi16(d0, d1);

				dr = _mm_or_si128(_mm_and_si128(emask, dr),
				    _mm_andnot_si128(emask, dd));

				_mm_storeu_si128((__m128i *) (tdp + i), dr);
			}
		} else if (blend == MLIB_BLEND_GTK_SRC) {
			mlib_u8 *buffi = (mlib_u8 *)buffz + 1;

			for (i = 0; i < width; i++) {
				tdp[0] = buffi[0];
				tdp[1] = buffi[2];
				tdp[2] = buffi[4];
				tdp[alp_ind] = 255;
				tdp += 4;
				buffi += 8;
			}
		} else {
			mlib_d64 _w0 = param->alpha;
			mlib_d64 _w1s = 1.0 - _w0 * (1.0 / 255);
			__m128i buff[1];
			__m128i done;
			__m128i dalp, ralp, ss, dd, s0, s1, d0, d1, a0, a1, r0,
			    r1, rr, dr;
			__m128i wi, aa, amask;
			__m128 af, w0, w1, w1s, w, rw, w0r, w1r, scale;

			done = _mm_set1_epi16(1 << 15);
			amask = _mm_set1_epi32(mask255);

			w0 = _mm_set_ps1(_w0);
			w1s = _mm_set_ps1(_w1s);
			scale = _mm_set_ps1(1 << 15);

			if (alp_ind == -1) {
				tdp--;
				for (i = 0; i < width / 4; i++) {
					BLEND34_SRC_OVER(0);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND34_SRC_OVER(0);
					buff[0] = dr;
				}
			} else {
				for (i = 0; i < width / 4; i++) {
					BLEND34_SRC_OVER(3);
					_mm_storeu_si128((__m128i *) tdp, dr);
					tdp += 16;
				}
				if (width & 3) {
					BLEND34_SRC_OVER(3);
					buff[0] = dr;
				}
			}
			for (i = 0; i < (width & 3); i++) {
				((mlib_s32 *)tdp)[i] = ((mlib_s32 *)buff)[i];
			}
		}
	} else if (chan_d == 3) {
		if (blend != MLIB_BLEND_GTK_SRC) {
			if (alp_ind == -1) {
				tdp--;
			}
			for (i = 0; i < width; i++) {
				((mlib_s32 *)buffd)[i] =
				    *(mlib_s32 *)(tdp + 3 * i);
			}

			if (alp_ind == -1) {
				for (i = 0; i < width2; i++) {
					__m128i a0, s0, d0, dd;

					BLEND43_SRC_OVER(0);
				}
				mlib_s_ImageChannelExtract_U8_43R_D1((void *)
				    buffd, dp, width);
			} else {
				for (i = 0; i < width2; i++) {
					__m128i a0, s0, d0, dd;

					BLEND43_SRC_OVER(0xff);
				}
				mlib_s_ImageChannelExtract_U8_43L_D1((void *)
				    buffd, dp, width);
			}
		} else {
			mlib_u8 *buffi = (mlib_u8 *)buffz + 1;

			if (alp_ind == -1)
				buffi += 2;
			for (i = 0; i < width; i++) {
				tdp[0] = buffi[0];
				tdp[1] = buffi[2];
				tdp[2] = buffi[4];
				tdp += 3;
				buffi += 8;
			}
		}
	} else {	/* if (chan_d == 4) */

		if (alp_ind == -1) {
			tdp--;
		}
		if (blend == MLIB_BLEND_GTK_SRC) {
			mlib_u8 *p_alp = (mlib_u8 *)buffz + 1;
			mlib_s32 tail = ((mlib_s32 *)tdp)[width];

			if (alp_ind != -1)
				p_alp += 6;
			for (i = 0; i < width2; i++) {
				__m128i a0, a1, aa, ss, d0, dd;

				ss = buffz[i];
				a0 = _mm_loadl_epi64((void *)((mlib_d64 *)
				    mlib_m_tbl_255DivAlpha + p_alp[0]));
				a1 = _mm_loadl_epi64((void *)((mlib_d64 *)
				    mlib_m_tbl_255DivAlpha + p_alp[8]));
				aa = _mm_unpacklo_epi64(a0, a1);
				aa = _mm_or_si128(amask256,
				    _mm_andnot_si128(amaskffff, aa));
				d0 = _mm_mulhi_epu16(ss, aa);
				dd = _mm_packus_epi16(d0, d0);
				_mm_storel_epi64((void *)(tdp + 8 * i), dd);
				p_alp += 16;
			}

			((mlib_s32 *)tdp)[width] = tail;
		} else {
			mlib_blend blend = param->blend;
			mlib_d64 alp = (param->alpha) * (1.0 / 255);
			__m128i buff[1];
			__m128i done;
			__m128i ss, dd, s0, s1, d0, d1, a0, a1, r0, r1, rr, dr;
			__m128i wi, aa, amask, a16mask, zero_mask_i;
			__m128 dalp, div255, alpha, fone;
			__m128 af, sf, w0, w1, w1s, w, rw, w0r, w1r, scale;
			__m128 zero_mask, f_rnd;
			mlib_m128 s0u, s1u, s2u, s3u;

			done = _mm_set1_epi16(1 << 14);
			amask = _mm_set1_epi32(mask255);
			a16mask = _mm_set1_epi32(0xFFFF);

			dalp = _mm_set_ps1(alp * (1.0 / 256));
			fone = _mm_set_ps1(1.0);
			div255 = _mm_set_ps1(1.0 / 255);
			scale = _mm_set_ps1(1 << 8);
			alpha = _mm_set_ps1((float)(param->alpha) + 0.5);
			f_rnd = _mm_set_ps1(0.6);

			if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
				if (alp_ind == -1) {
					for (i = 0; i < width / 4; i++) {
						BLEND44(SRC_OVER2, 0);
						_mm_storeu_si128((__m128i *)
						    tdp, dr);
						tdp += 16;
					}
					if (width & 3) {
						BLEND44(SRC_OVER2, 0);
						buff[0] = dr;
					}
				} else {
					for (i = 0; i < width / 4; i++) {
						BLEND44(SRC_OVER2, 3);
						_mm_storeu_si128((__m128i *)
						    tdp, dr);
						tdp += 16;
					}
					if (width & 3) {
						BLEND44(SRC_OVER2, 3);
						buff[0] = dr;
					}
				}
			} else {
				if (alp_ind == -1) {
					for (i = 0; i < width / 4; i++) {
						BLEND44(SRC_OVER, 0);
						_mm_storeu_si128((__m128i *)
						    tdp, dr);
						tdp += 16;
					}
					if (width & 3) {
						BLEND44(SRC_OVER, 0);
						buff[0] = dr;
					}
				} else {
					for (i = 0; i < width / 4; i++) {
						BLEND44(SRC_OVER, 3);
						_mm_storeu_si128((__m128i *)
						    tdp, dr);
						tdp += 16;
					}
					if (width & 3) {
						BLEND44(SRC_OVER, 3);
						buff[0] = dr;
					}
				}
			}

			for (i = 0; i < (width & 3); i++) {
				((mlib_s32 *)tdp)[i] = ((mlib_s32 *)buff)[i];
			}
		}
	}
}

/* *********************************************************** */

static const mlib_zoomblend_fun_type mlib_zoom_nn_funs[] = {
	mlib_ImageZoom_U8_33_Nearest,
	mlib_ImageZoom_U8_43_Nearest,
	mlib_ImageZoom_U8_34_Nearest,
	mlib_ImageZoom_U8_44_Nearest,
};

/* *********************************************************** */

mlib_status
mlib_ImageZoomTranslateBlend_intern(
    mlib_work_image * param,
    mlib_filter filter,
    mlib_d64 zoomy,
    mlib_s32 t_ind)
{
	const mlib_f32 *flt_table;

	switch (filter) {
	case MLIB_NEAREST:
		return mlib_zoom_nn_funs[t_ind] (param, param->current->sp,
		    param->current->dp);

	case MLIB_BILINEAR:
		return (mlib_s_ImageZoomBlend_BL_U8(param));

	case MLIB_BICUBIC:
	case MLIB_BICUBIC2:
		if (filter == MLIB_BICUBIC) {
			param->filter1 = (void *)mlib_filters_u8_bc;
		} else {
			param->filter1 = (void *)mlib_filters_u8_bc2;
		}
		return (mlib_s_ImageZoomBlend_BC_U8(param));

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
