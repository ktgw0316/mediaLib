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

#pragma ident	"@(#)mlib_v_ImageBlendColor_U8.c	9.2	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageBlendColor on U8 data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageBlendColor.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageMulAlpha.h>

/* *********************************************************** */

static const mlib_u64 mlib_dmask_arr[] = {
	0x0000000000000000, 0x000000000000FFFF, 0x00000000FFFF0000,
	    0x00000000FFFFFFFF,
	0x0000FFFF00000000, 0x0000FFFF0000FFFF, 0x0000FFFFFFFF0000,
	    0x0000FFFFFFFFFFFF,
	0xFFFF000000000000, 0xFFFF00000000FFFF, 0xFFFF0000FFFF0000,
	    0xFFFF0000FFFFFFFF,
	0xFFFFFFFF00000000, 0xFFFFFFFF0000FFFF, 0xFFFFFFFFFFFF0000,
	    0xFFFFFFFFFFFFFFFF
};

/* *********************************************************** */

#define	MEM_COPY(src, dst, n)	mlib_ImageCopy_na((void*)src, (void*)dst, n)

/* *********************************************************** */

#define	DOUBLE_4U16(x0, x1, x2, x3)                             \
	vis_to_double((((x0 & 0xFF) << 16) | (x1 & 0xFF)),      \
	    (((x2 & 0xFF) << 16) | (x3 & 0xFF)))

/* *********************************************************** */

static const mlib_u32 mlib_alp_tbl[] = {
	0x00000000, 0x00010101, 0x00020202, 0x00030303,
	0x00040404, 0x00050505, 0x00060606, 0x00070707,
	0x00080808, 0x00090909, 0x000a0a0a, 0x000b0b0b,
	0x000c0c0c, 0x000d0d0d, 0x000e0e0e, 0x000f0f0f,
	0x00101010, 0x00111111, 0x00121212, 0x00131313,
	0x00141414, 0x00151515, 0x00161616, 0x00171717,
	0x00181818, 0x00191919, 0x001a1a1a, 0x001b1b1b,
	0x001c1c1c, 0x001d1d1d, 0x001e1e1e, 0x001f1f1f,
	0x00202020, 0x00212121, 0x00222222, 0x00232323,
	0x00242424, 0x00252525, 0x00262626, 0x00272727,
	0x00282828, 0x00292929, 0x002a2a2a, 0x002b2b2b,
	0x002c2c2c, 0x002d2d2d, 0x002e2e2e, 0x002f2f2f,
	0x00303030, 0x00313131, 0x00323232, 0x00333333,
	0x00343434, 0x00353535, 0x00363636, 0x00373737,
	0x00383838, 0x00393939, 0x003a3a3a, 0x003b3b3b,
	0x003c3c3c, 0x003d3d3d, 0x003e3e3e, 0x003f3f3f,
	0x00404040, 0x00414141, 0x00424242, 0x00434343,
	0x00444444, 0x00454545, 0x00464646, 0x00474747,
	0x00484848, 0x00494949, 0x004a4a4a, 0x004b4b4b,
	0x004c4c4c, 0x004d4d4d, 0x004e4e4e, 0x004f4f4f,
	0x00505050, 0x00515151, 0x00525252, 0x00535353,
	0x00545454, 0x00555555, 0x00565656, 0x00575757,
	0x00585858, 0x00595959, 0x005a5a5a, 0x005b5b5b,
	0x005c5c5c, 0x005d5d5d, 0x005e5e5e, 0x005f5f5f,
	0x00606060, 0x00616161, 0x00626262, 0x00636363,
	0x00646464, 0x00656565, 0x00666666, 0x00676767,
	0x00686868, 0x00696969, 0x006a6a6a, 0x006b6b6b,
	0x006c6c6c, 0x006d6d6d, 0x006e6e6e, 0x006f6f6f,
	0x00707070, 0x00717171, 0x00727272, 0x00737373,
	0x00747474, 0x00757575, 0x00767676, 0x00777777,
	0x00787878, 0x00797979, 0x007a7a7a, 0x007b7b7b,
	0x007c7c7c, 0x007d7d7d, 0x007e7e7e, 0x007f7f7f,
	0x00808080, 0x00818181, 0x00828282, 0x00838383,
	0x00848484, 0x00858585, 0x00868686, 0x00878787,
	0x00888888, 0x00898989, 0x008a8a8a, 0x008b8b8b,
	0x008c8c8c, 0x008d8d8d, 0x008e8e8e, 0x008f8f8f,
	0x00909090, 0x00919191, 0x00929292, 0x00939393,
	0x00949494, 0x00959595, 0x00969696, 0x00979797,
	0x00989898, 0x00999999, 0x009a9a9a, 0x009b9b9b,
	0x009c9c9c, 0x009d9d9d, 0x009e9e9e, 0x009f9f9f,
	0x00a0a0a0, 0x00a1a1a1, 0x00a2a2a2, 0x00a3a3a3,
	0x00a4a4a4, 0x00a5a5a5, 0x00a6a6a6, 0x00a7a7a7,
	0x00a8a8a8, 0x00a9a9a9, 0x00aaaaaa, 0x00ababab,
	0x00acacac, 0x00adadad, 0x00aeaeae, 0x00afafaf,
	0x00b0b0b0, 0x00b1b1b1, 0x00b2b2b2, 0x00b3b3b3,
	0x00b4b4b4, 0x00b5b5b5, 0x00b6b6b6, 0x00b7b7b7,
	0x00b8b8b8, 0x00b9b9b9, 0x00bababa, 0x00bbbbbb,
	0x00bcbcbc, 0x00bdbdbd, 0x00bebebe, 0x00bfbfbf,
	0x00c0c0c0, 0x00c1c1c1, 0x00c2c2c2, 0x00c3c3c3,
	0x00c4c4c4, 0x00c5c5c5, 0x00c6c6c6, 0x00c7c7c7,
	0x00c8c8c8, 0x00c9c9c9, 0x00cacaca, 0x00cbcbcb,
	0x00cccccc, 0x00cdcdcd, 0x00cecece, 0x00cfcfcf,
	0x00d0d0d0, 0x00d1d1d1, 0x00d2d2d2, 0x00d3d3d3,
	0x00d4d4d4, 0x00d5d5d5, 0x00d6d6d6, 0x00d7d7d7,
	0x00d8d8d8, 0x00d9d9d9, 0x00dadada, 0x00dbdbdb,
	0x00dcdcdc, 0x00dddddd, 0x00dedede, 0x00dfdfdf,
	0x00e0e0e0, 0x00e1e1e1, 0x00e2e2e2, 0x00e3e3e3,
	0x00e4e4e4, 0x00e5e5e5, 0x00e6e6e6, 0x00e7e7e7,
	0x00e8e8e8, 0x00e9e9e9, 0x00eaeaea, 0x00ebebeb,
	0x00ececec, 0x00ededed, 0x00eeeeee, 0x00efefef,
	0x00f0f0f0, 0x00f1f1f1, 0x00f2f2f2, 0x00f3f3f3,
	0x00f4f4f4, 0x00f5f5f5, 0x00f6f6f6, 0x00f7f7f7,
	0x00f8f8f8, 0x00f9f9f9, 0x00fafafa, 0x00fbfbfb,
	0x00fcfcfc, 0x00fdfdfd, 0x00fefefe, 0x00ffffff,

	0x00000000, 0x01000101, 0x02000202, 0x03000303,
	0x04000404, 0x05000505, 0x06000606, 0x07000707,
	0x08000808, 0x09000909, 0x0a000a0a, 0x0b000b0b,
	0x0c000c0c, 0x0d000d0d, 0x0e000e0e, 0x0f000f0f,
	0x10001010, 0x11001111, 0x12001212, 0x13001313,
	0x14001414, 0x15001515, 0x16001616, 0x17001717,
	0x18001818, 0x19001919, 0x1a001a1a, 0x1b001b1b,
	0x1c001c1c, 0x1d001d1d, 0x1e001e1e, 0x1f001f1f,
	0x20002020, 0x21002121, 0x22002222, 0x23002323,
	0x24002424, 0x25002525, 0x26002626, 0x27002727,
	0x28002828, 0x29002929, 0x2a002a2a, 0x2b002b2b,
	0x2c002c2c, 0x2d002d2d, 0x2e002e2e, 0x2f002f2f,
	0x30003030, 0x31003131, 0x32003232, 0x33003333,
	0x34003434, 0x35003535, 0x36003636, 0x37003737,
	0x38003838, 0x39003939, 0x3a003a3a, 0x3b003b3b,
	0x3c003c3c, 0x3d003d3d, 0x3e003e3e, 0x3f003f3f,
	0x40004040, 0x41004141, 0x42004242, 0x43004343,
	0x44004444, 0x45004545, 0x46004646, 0x47004747,
	0x48004848, 0x49004949, 0x4a004a4a, 0x4b004b4b,
	0x4c004c4c, 0x4d004d4d, 0x4e004e4e, 0x4f004f4f,
	0x50005050, 0x51005151, 0x52005252, 0x53005353,
	0x54005454, 0x55005555, 0x56005656, 0x57005757,
	0x58005858, 0x59005959, 0x5a005a5a, 0x5b005b5b,
	0x5c005c5c, 0x5d005d5d, 0x5e005e5e, 0x5f005f5f,
	0x60006060, 0x61006161, 0x62006262, 0x63006363,
	0x64006464, 0x65006565, 0x66006666, 0x67006767,
	0x68006868, 0x69006969, 0x6a006a6a, 0x6b006b6b,
	0x6c006c6c, 0x6d006d6d, 0x6e006e6e, 0x6f006f6f,
	0x70007070, 0x71007171, 0x72007272, 0x73007373,
	0x74007474, 0x75007575, 0x76007676, 0x77007777,
	0x78007878, 0x79007979, 0x7a007a7a, 0x7b007b7b,
	0x7c007c7c, 0x7d007d7d, 0x7e007e7e, 0x7f007f7f,
	0x80008080, 0x81008181, 0x82008282, 0x83008383,
	0x84008484, 0x85008585, 0x86008686, 0x87008787,
	0x88008888, 0x89008989, 0x8a008a8a, 0x8b008b8b,
	0x8c008c8c, 0x8d008d8d, 0x8e008e8e, 0x8f008f8f,
	0x90009090, 0x91009191, 0x92009292, 0x93009393,
	0x94009494, 0x95009595, 0x96009696, 0x97009797,
	0x98009898, 0x99009999, 0x9a009a9a, 0x9b009b9b,
	0x9c009c9c, 0x9d009d9d, 0x9e009e9e, 0x9f009f9f,
	0xa000a0a0, 0xa100a1a1, 0xa200a2a2, 0xa300a3a3,
	0xa400a4a4, 0xa500a5a5, 0xa600a6a6, 0xa700a7a7,
	0xa800a8a8, 0xa900a9a9, 0xaa00aaaa, 0xab00abab,
	0xac00acac, 0xad00adad, 0xae00aeae, 0xaf00afaf,
	0xb000b0b0, 0xb100b1b1, 0xb200b2b2, 0xb300b3b3,
	0xb400b4b4, 0xb500b5b5, 0xb600b6b6, 0xb700b7b7,
	0xb800b8b8, 0xb900b9b9, 0xba00baba, 0xbb00bbbb,
	0xbc00bcbc, 0xbd00bdbd, 0xbe00bebe, 0xbf00bfbf,
	0xc000c0c0, 0xc100c1c1, 0xc200c2c2, 0xc300c3c3,
	0xc400c4c4, 0xc500c5c5, 0xc600c6c6, 0xc700c7c7,
	0xc800c8c8, 0xc900c9c9, 0xca00caca, 0xcb00cbcb,
	0xcc00cccc, 0xcd00cdcd, 0xce00cece, 0xcf00cfcf,
	0xd000d0d0, 0xd100d1d1, 0xd200d2d2, 0xd300d3d3,
	0xd400d4d4, 0xd500d5d5, 0xd600d6d6, 0xd700d7d7,
	0xd800d8d8, 0xd900d9d9, 0xda00dada, 0xdb00dbdb,
	0xdc00dcdc, 0xdd00dddd, 0xde00dede, 0xdf00dfdf,
	0xe000e0e0, 0xe100e1e1, 0xe200e2e2, 0xe300e3e3,
	0xe400e4e4, 0xe500e5e5, 0xe600e6e6, 0xe700e7e7,
	0xe800e8e8, 0xe900e9e9, 0xea00eaea, 0xeb00ebeb,
	0xec00ecec, 0xed00eded, 0xee00eeee, 0xef00efef,
	0xf000f0f0, 0xf100f1f1, 0xf200f2f2, 0xf300f3f3,
	0xf400f4f4, 0xf500f5f5, 0xf600f6f6, 0xf700f7f7,
	0xf800f8f8, 0xf900f9f9, 0xfa00fafa, 0xfb00fbfb,
	0xfc00fcfc, 0xfd00fdfd, 0xfe00fefe, 0xff00ffff,

	0x00000000, 0x01010001, 0x02020002, 0x03030003,
	0x04040004, 0x05050005, 0x06060006, 0x07070007,
	0x08080008, 0x09090009, 0x0a0a000a, 0x0b0b000b,
	0x0c0c000c, 0x0d0d000d, 0x0e0e000e, 0x0f0f000f,
	0x10100010, 0x11110011, 0x12120012, 0x13130013,
	0x14140014, 0x15150015, 0x16160016, 0x17170017,
	0x18180018, 0x19190019, 0x1a1a001a, 0x1b1b001b,
	0x1c1c001c, 0x1d1d001d, 0x1e1e001e, 0x1f1f001f,
	0x20200020, 0x21210021, 0x22220022, 0x23230023,
	0x24240024, 0x25250025, 0x26260026, 0x27270027,
	0x28280028, 0x29290029, 0x2a2a002a, 0x2b2b002b,
	0x2c2c002c, 0x2d2d002d, 0x2e2e002e, 0x2f2f002f,
	0x30300030, 0x31310031, 0x32320032, 0x33330033,
	0x34340034, 0x35350035, 0x36360036, 0x37370037,
	0x38380038, 0x39390039, 0x3a3a003a, 0x3b3b003b,
	0x3c3c003c, 0x3d3d003d, 0x3e3e003e, 0x3f3f003f,
	0x40400040, 0x41410041, 0x42420042, 0x43430043,
	0x44440044, 0x45450045, 0x46460046, 0x47470047,
	0x48480048, 0x49490049, 0x4a4a004a, 0x4b4b004b,
	0x4c4c004c, 0x4d4d004d, 0x4e4e004e, 0x4f4f004f,
	0x50500050, 0x51510051, 0x52520052, 0x53530053,
	0x54540054, 0x55550055, 0x56560056, 0x57570057,
	0x58580058, 0x59590059, 0x5a5a005a, 0x5b5b005b,
	0x5c5c005c, 0x5d5d005d, 0x5e5e005e, 0x5f5f005f,
	0x60600060, 0x61610061, 0x62620062, 0x63630063,
	0x64640064, 0x65650065, 0x66660066, 0x67670067,
	0x68680068, 0x69690069, 0x6a6a006a, 0x6b6b006b,
	0x6c6c006c, 0x6d6d006d, 0x6e6e006e, 0x6f6f006f,
	0x70700070, 0x71710071, 0x72720072, 0x73730073,
	0x74740074, 0x75750075, 0x76760076, 0x77770077,
	0x78780078, 0x79790079, 0x7a7a007a, 0x7b7b007b,
	0x7c7c007c, 0x7d7d007d, 0x7e7e007e, 0x7f7f007f,
	0x80800080, 0x81810081, 0x82820082, 0x83830083,
	0x84840084, 0x85850085, 0x86860086, 0x87870087,
	0x88880088, 0x89890089, 0x8a8a008a, 0x8b8b008b,
	0x8c8c008c, 0x8d8d008d, 0x8e8e008e, 0x8f8f008f,
	0x90900090, 0x91910091, 0x92920092, 0x93930093,
	0x94940094, 0x95950095, 0x96960096, 0x97970097,
	0x98980098, 0x99990099, 0x9a9a009a, 0x9b9b009b,
	0x9c9c009c, 0x9d9d009d, 0x9e9e009e, 0x9f9f009f,
	0xa0a000a0, 0xa1a100a1, 0xa2a200a2, 0xa3a300a3,
	0xa4a400a4, 0xa5a500a5, 0xa6a600a6, 0xa7a700a7,
	0xa8a800a8, 0xa9a900a9, 0xaaaa00aa, 0xabab00ab,
	0xacac00ac, 0xadad00ad, 0xaeae00ae, 0xafaf00af,
	0xb0b000b0, 0xb1b100b1, 0xb2b200b2, 0xb3b300b3,
	0xb4b400b4, 0xb5b500b5, 0xb6b600b6, 0xb7b700b7,
	0xb8b800b8, 0xb9b900b9, 0xbaba00ba, 0xbbbb00bb,
	0xbcbc00bc, 0xbdbd00bd, 0xbebe00be, 0xbfbf00bf,
	0xc0c000c0, 0xc1c100c1, 0xc2c200c2, 0xc3c300c3,
	0xc4c400c4, 0xc5c500c5, 0xc6c600c6, 0xc7c700c7,
	0xc8c800c8, 0xc9c900c9, 0xcaca00ca, 0xcbcb00cb,
	0xcccc00cc, 0xcdcd00cd, 0xcece00ce, 0xcfcf00cf,
	0xd0d000d0, 0xd1d100d1, 0xd2d200d2, 0xd3d300d3,
	0xd4d400d4, 0xd5d500d5, 0xd6d600d6, 0xd7d700d7,
	0xd8d800d8, 0xd9d900d9, 0xdada00da, 0xdbdb00db,
	0xdcdc00dc, 0xdddd00dd, 0xdede00de, 0xdfdf00df,
	0xe0e000e0, 0xe1e100e1, 0xe2e200e2, 0xe3e300e3,
	0xe4e400e4, 0xe5e500e5, 0xe6e600e6, 0xe7e700e7,
	0xe8e800e8, 0xe9e900e9, 0xeaea00ea, 0xebeb00eb,
	0xecec00ec, 0xeded00ed, 0xeeee00ee, 0xefef00ef,
	0xf0f000f0, 0xf1f100f1, 0xf2f200f2, 0xf3f300f3,
	0xf4f400f4, 0xf5f500f5, 0xf6f600f6, 0xf7f700f7,
	0xf8f800f8, 0xf9f900f9, 0xfafa00fa, 0xfbfb00fb,
	0xfcfc00fc, 0xfdfd00fd, 0xfefe00fe, 0xffff00ff,

	0x00000000, 0x01010100, 0x02020200, 0x03030300,
	0x04040400, 0x05050500, 0x06060600, 0x07070700,
	0x08080800, 0x09090900, 0x0a0a0a00, 0x0b0b0b00,
	0x0c0c0c00, 0x0d0d0d00, 0x0e0e0e00, 0x0f0f0f00,
	0x10101000, 0x11111100, 0x12121200, 0x13131300,
	0x14141400, 0x15151500, 0x16161600, 0x17171700,
	0x18181800, 0x19191900, 0x1a1a1a00, 0x1b1b1b00,
	0x1c1c1c00, 0x1d1d1d00, 0x1e1e1e00, 0x1f1f1f00,
	0x20202000, 0x21212100, 0x22222200, 0x23232300,
	0x24242400, 0x25252500, 0x26262600, 0x27272700,
	0x28282800, 0x29292900, 0x2a2a2a00, 0x2b2b2b00,
	0x2c2c2c00, 0x2d2d2d00, 0x2e2e2e00, 0x2f2f2f00,
	0x30303000, 0x31313100, 0x32323200, 0x33333300,
	0x34343400, 0x35353500, 0x36363600, 0x37373700,
	0x38383800, 0x39393900, 0x3a3a3a00, 0x3b3b3b00,
	0x3c3c3c00, 0x3d3d3d00, 0x3e3e3e00, 0x3f3f3f00,
	0x40404000, 0x41414100, 0x42424200, 0x43434300,
	0x44444400, 0x45454500, 0x46464600, 0x47474700,
	0x48484800, 0x49494900, 0x4a4a4a00, 0x4b4b4b00,
	0x4c4c4c00, 0x4d4d4d00, 0x4e4e4e00, 0x4f4f4f00,
	0x50505000, 0x51515100, 0x52525200, 0x53535300,
	0x54545400, 0x55555500, 0x56565600, 0x57575700,
	0x58585800, 0x59595900, 0x5a5a5a00, 0x5b5b5b00,
	0x5c5c5c00, 0x5d5d5d00, 0x5e5e5e00, 0x5f5f5f00,
	0x60606000, 0x61616100, 0x62626200, 0x63636300,
	0x64646400, 0x65656500, 0x66666600, 0x67676700,
	0x68686800, 0x69696900, 0x6a6a6a00, 0x6b6b6b00,
	0x6c6c6c00, 0x6d6d6d00, 0x6e6e6e00, 0x6f6f6f00,
	0x70707000, 0x71717100, 0x72727200, 0x73737300,
	0x74747400, 0x75757500, 0x76767600, 0x77777700,
	0x78787800, 0x79797900, 0x7a7a7a00, 0x7b7b7b00,
	0x7c7c7c00, 0x7d7d7d00, 0x7e7e7e00, 0x7f7f7f00,
	0x80808000, 0x81818100, 0x82828200, 0x83838300,
	0x84848400, 0x85858500, 0x86868600, 0x87878700,
	0x88888800, 0x89898900, 0x8a8a8a00, 0x8b8b8b00,
	0x8c8c8c00, 0x8d8d8d00, 0x8e8e8e00, 0x8f8f8f00,
	0x90909000, 0x91919100, 0x92929200, 0x93939300,
	0x94949400, 0x95959500, 0x96969600, 0x97979700,
	0x98989800, 0x99999900, 0x9a9a9a00, 0x9b9b9b00,
	0x9c9c9c00, 0x9d9d9d00, 0x9e9e9e00, 0x9f9f9f00,
	0xa0a0a000, 0xa1a1a100, 0xa2a2a200, 0xa3a3a300,
	0xa4a4a400, 0xa5a5a500, 0xa6a6a600, 0xa7a7a700,
	0xa8a8a800, 0xa9a9a900, 0xaaaaaa00, 0xababab00,
	0xacacac00, 0xadadad00, 0xaeaeae00, 0xafafaf00,
	0xb0b0b000, 0xb1b1b100, 0xb2b2b200, 0xb3b3b300,
	0xb4b4b400, 0xb5b5b500, 0xb6b6b600, 0xb7b7b700,
	0xb8b8b800, 0xb9b9b900, 0xbababa00, 0xbbbbbb00,
	0xbcbcbc00, 0xbdbdbd00, 0xbebebe00, 0xbfbfbf00,
	0xc0c0c000, 0xc1c1c100, 0xc2c2c200, 0xc3c3c300,
	0xc4c4c400, 0xc5c5c500, 0xc6c6c600, 0xc7c7c700,
	0xc8c8c800, 0xc9c9c900, 0xcacaca00, 0xcbcbcb00,
	0xcccccc00, 0xcdcdcd00, 0xcecece00, 0xcfcfcf00,
	0xd0d0d000, 0xd1d1d100, 0xd2d2d200, 0xd3d3d300,
	0xd4d4d400, 0xd5d5d500, 0xd6d6d600, 0xd7d7d700,
	0xd8d8d800, 0xd9d9d900, 0xdadada00, 0xdbdbdb00,
	0xdcdcdc00, 0xdddddd00, 0xdedede00, 0xdfdfdf00,
	0xe0e0e000, 0xe1e1e100, 0xe2e2e200, 0xe3e3e300,
	0xe4e4e400, 0xe5e5e500, 0xe6e6e600, 0xe7e7e700,
	0xe8e8e800, 0xe9e9e900, 0xeaeaea00, 0xebebeb00,
	0xececec00, 0xededed00, 0xeeeeee00, 0xefefef00,
	0xf0f0f000, 0xf1f1f100, 0xf2f2f200, 0xf3f3f300,
	0xf4f4f400, 0xf5f5f500, 0xf6f6f600, 0xf7f7f700,
	0xf8f8f800, 0xf9f9f900, 0xfafafa00, 0xfbfbfb00,
	0xfcfcfc00, 0xfdfdfd00, 0xfefefe00, 0xffffff00
};

/* *********************************************************** */

#define	EXPAND0(x)	vis_fpmerge(fzeros, x)
#define	EXPAND1(x)	vis_fmul8x16al(x, fzeros)
#define	EXPAND2(x)	vis_fexpand(x)

/* *********************************************************** */

#define	BLEND_COLOR(rr, src, alp, cc, EXPAND)                   \
	rr = EXPAND(src);                                       \
	rr = vis_fpsub16(rr, cc);                               \
	rr = vis_fmul8x16(alp, rr);                             \
	rr = vis_fpadd16(rr, cc)

/* *********************************************************** */

#define	BLEND_COLOR_2CH_0()                                     \
	rr = vis_fand(ss, dmask);                               \
	rr = vis_fpsub16(rr, cc);                               \
	s1 = vis_fpmerge(vis_read_hi(ss), vis_read_lo(ss));     \
	s1 = vis_fpmerge(vis_read_hi(s1), vis_read_lo(s1));     \
	rr = vis_fmul8x16(vis_read_hi(s1), rr);                 \
	rr = vis_fpadd16(rr, cc);                               \
	rr = vis_fpmerge(fmax, vis_fpack16(rr))

/* *********************************************************** */

#define	BLEND_COLOR_2CH_1()                                     \
	rr = vis_fand(ss, dmask);                               \
	tt = vis_fpsub16(done, rr);                             \
	s1 = vis_fpmerge(vis_read_hi(ss), vis_read_lo(ss));     \
	s1 = vis_fpmerge(vis_read_hi(s1), vis_read_lo(s1));     \
	rr = vis_fmul8x16(vis_read_hi(s1), rr);                 \
	tt = vis_fmul8x16(vis_read_hi(cc), tt);                 \
	rr = vis_fpadd16(rr, tt);                               \
	rr = vis_fpmerge(vis_fpack16(rr), fmax)

/* *********************************************************** */

#define	LOAD_3CH_0()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	a0 = vis_fand(s0, amask0);                              \
	a1 = vis_fand(s1, amask1);                              \
	a2 = vis_fand(s2, amask2);                              \
	a2 = vis_faligndata(a1, a2);                            \
	a1 = vis_faligndata(a0, a1);                            \
	a0 = vis_faligndata(a0, a0);                            \
	a2 = vis_for(a2, vis_faligndata(a1, a2));               \
	a1 = vis_for(a1, vis_faligndata(a0, a1));               \
	a0 = vis_for(a0, vis_faligndata(done, a0))

/* *********************************************************** */

#define	LOAD_3CH_1()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	a0 = vis_fand(s0, amask0);                              \
	a1 = vis_fand(s1, amask1);                              \
	a2 = vis_fand(s2, amask2);                              \
	vis_alignaddr((void *)0, 1);                            \
	b0 = vis_faligndata(a0, a0);                            \
	b1 = vis_faligndata(a1, a2);                            \
	b2 = vis_faligndata(a2, a0);                            \
	vis_alignaddr((void *)0, 7);                            \
	a2 = vis_for(b2, vis_faligndata(a1, a2));               \
	a1 = vis_for(b1, vis_faligndata(a0, a1));               \
	a0 = vis_for(b0, vis_faligndata(done, a0))

/* *********************************************************** */

#define	LOAD_3CH_2()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	a0 = vis_fand(s0, amask0);                              \
	a1 = vis_fand(s1, amask1);                              \
	a2 = vis_fand(s2, amask2);                              \
	a0 = vis_faligndata(a0, a1);                            \
	a1 = vis_faligndata(a1, a2);                            \
	a2 = vis_faligndata(a2, a2);                            \
	a0 = vis_for(a0, vis_faligndata(a0, a1));               \
	a1 = vis_for(a1, vis_faligndata(a1, a2));               \
	a2 = vis_for(a2, vis_faligndata(a2, amask0))

/* *********************************************************** */

#define	BLEND_COLOR_3CH()                                                 \
	BLEND_COLOR(d0, vis_read_hi(s0), vis_read_hi(a0), c0, EXPAND0);   \
	BLEND_COLOR(d1, vis_read_lo(s0), vis_read_lo(a0), c1, EXPAND0);   \
	dp[0] = vis_fpack16_pair(d0, d1);                                 \
	BLEND_COLOR(d0, vis_read_hi(s1), vis_read_hi(a1), c2, EXPAND0);   \
	BLEND_COLOR(d1, vis_read_lo(s1), vis_read_lo(a1), c0, EXPAND0);   \
	dp[1] = vis_fpack16_pair(d0, d1);                                 \
	BLEND_COLOR(d0, vis_read_hi(s2), vis_read_hi(a2), c1, EXPAND0);   \
	BLEND_COLOR(d1, vis_read_lo(s2), vis_read_lo(a2), c2, EXPAND0);   \
	dp[2] = vis_fpack16_pair(d0, d1);                                 \
	sp += 3;                                                          \
	dp += 3

/* *********************************************************** */

DEF_FUNC(mlib_ImageBlendColor_U8, mlib_u8,
    mlib_s32)
{
	mlib_f32 fzeros = vis_fzeros();
	mlib_f32 fmax = vis_to_float(0xFFFFFFFF);
	mlib_d64 dmask = vis_to_double_dup(0x00FF00FF);
	mlib_d64 done = vis_to_double_dup(0x01000100);
	mlib_d64 *buffs, *buffd;
	mlib_d64 *sp, *dp;
	mlib_f32 *alp_tbl;
	mlib_d64 ss, s1, rr, tt, d0, d1;
	mlib_d64 cc, c0, c1, c2;
	mlib_d64 amask0, amask1, amask2;
	mlib_s32 ww, dflag, i, j;

	vis_write_gsr(7 << 3);

	width *= channel;
	ww = (width + 7) / 8;

	if (channel == 3) {
		ww = 3 * ((ww + 2) / 3);
	}

	buffs = __mlib_malloc(2 * sizeof (mlib_d64) * ww);

	if (buffs == NULL) {
		return (MLIB_FAILURE);
	}

	buffd = buffs + ww;

	if (channel == 4) {
		cc = DOUBLE_4U16(color[0], color[1], color[2], color[3]);
		cc = vis_fand(vis_for(cc,
		    ((mlib_d64 *)mlib_dmask_arr)[8 >> alpha]), dmask);
		alp_tbl = (mlib_f32 *)mlib_alp_tbl + alpha * 256;
	} else if (channel == 3) {
		mlib_d64 c_arr[3];
		mlib_s16 *p_arr = (mlib_s16 *)c_arr;

		amask0 = ((mlib_d64 *)mlib_amask3_arr)[alpha];
		amask1 = ((mlib_d64 *)mlib_amask3_arr)[alpha + 1];
		amask2 = ((mlib_d64 *)mlib_amask3_arr)[alpha + 2];
		p_arr[0] = color[0] & 0xFF;
		p_arr[1] = color[1] & 0xFF;
		p_arr[2] = color[2] & 0xFF;
		p_arr[alpha] = 0xFF;
		for (i = 0; i < 3; i++) {
			p_arr[i + 3] = p_arr[i + 6] = p_arr[i + 9] = p_arr[i];
		}

		c0 = c_arr[0];
		c1 = c_arr[1];
		c2 = c_arr[2];
	} else {	/* if (channel == 2) */

		if (alpha == 0) {
			mlib_s32 col = color[1] & 0xFF;

			cc = vis_to_double_dup((col << 16) | col);
		} else {
			mlib_s32 col = color[0] & 0xFF;

			cc = vis_to_double_dup((col << 24) | (col << 16) | (col
			    << 8) | col);
		}
	}

	for (j = 0; j < height; j++) {
		if (((int)sl & 7)) {
			MEM_COPY(sl, buffs, width);
			sp = buffs;
		} else {
			sp = (mlib_d64 *)sl;
		}

		dflag = 0;

		if (((int)dl | width) & 7) {
			dp = buffd;
			dflag = 1;
		} else {
			dp = (mlib_d64 *)dl;
		}

		if (channel == 4) {
			mlib_f32 a0, a1;

#pragma pipeloop(0)
			for (i = 0; i < ww; i++) {
				a0 = alp_tbl[((mlib_u8 *)sp)[alpha]];
				a1 = alp_tbl[((mlib_u8 *)sp)[alpha + 4]];
				ss = (*sp++);
				BLEND_COLOR(d0, vis_read_hi(ss), a0, cc,
				    EXPAND0);
				BLEND_COLOR(d1, vis_read_lo(ss), a1, cc,
				    EXPAND0);
				(*dp++) = vis_fpack16_pair(d0, d1);
			}

		} else if (channel == 3) {
			mlib_d64 s0, s1, s2;
			mlib_d64 a0, a1, a2;

			if (alpha == 0) {
				vis_alignaddr((void *)0, 7);
#pragma pipeloop(0)
				for (i = 0; i < ww; i += 3) {
					LOAD_3CH_0();
					BLEND_COLOR_3CH();
				}

			} else if (alpha == 1) {
				mlib_d64 b0, b1, b2;

#pragma pipeloop(0)
				for (i = 0; i < ww; i += 3) {
					LOAD_3CH_1();
					BLEND_COLOR_3CH();
				}

			} else {	/* if (alpha == 2) */

				vis_alignaddr((void *)0, 1);
#pragma pipeloop(0)
				for (i = 0; i < ww; i += 3) {
					LOAD_3CH_2();
					BLEND_COLOR_3CH();
				}
			}

		} else {	/* if (channel == 2) */

			if (alpha == 0) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					BLEND_COLOR_2CH_0();
					(*dp++) = rr;
				}

			} else {	/* if (alpha == 1) */

#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					BLEND_COLOR_2CH_1();
					(*dp++) = rr;
				}
			}
		}

		if (dflag) {
			MEM_COPY(buffd, dl, width);
		}

		sl += sstride;
		dl += dstride;
	}

	__mlib_free(buffs);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
