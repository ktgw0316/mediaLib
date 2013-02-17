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

#pragma ident	"@(#)mlib_ImageFlipMainDiag_Bit.c	9.3	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImageFlipMainDiag.h>

/* *********************************************************** */

static mlib_status mlib_ImageFlipMainDiag_BIT_1_AA(
    const mlib_u8 *sdata,
    mlib_s32 src_stride,
    mlib_u8 *ddata,
    mlib_s32 dst_stride,
    mlib_s32 pix_sh,
    mlib_s32 pix_sw);

static mlib_status mlib_ImageFlipMainDiag_BIT_1_GE(
    const mlib_u8 *sdata,
    mlib_s32 src_stride,
    mlib_u8 *ddata,
    mlib_s32 dst_stride,
    mlib_s32 pix_sh,
    mlib_s32 pix_sw,
    mlib_s32 src_bitoffset,
    mlib_s32 dst_bitoffset);

/* *********************************************************** */

#define	BUF_SIZE	256

/* *********************************************************** */

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
#define	TABLE	(mlib_FlipAndFixRotateTable)
#else /* _NO_LONGLONG || _LITTLE_ENDIAN */

#define	TABLE	((mlib_u64 *)mlib_FlipAndFixRotateTable)
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

/* *********************************************************** */

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

#define	STORE_VAR	mlib_u32 r0, r1

#define	CLEAR_VAR	r0 = 0; r1 = 0

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	TRANSP_0(val, shift)                                    \
	r0 = *(TABLE + 2 * val + 1) >> shift;                   \
	r1 = *(TABLE + 2 * val + 0) >> shift

/* *********************************************************** */

#define	TRANSP_1(val, shift)                                    \
	r0 |= *(TABLE + 2 * val + 1) >> shift;                  \
	r1 |= *(TABLE + 2 * val + 0) >> shift

/* *********************************************************** */

#define	STORE(ptr, stride)                                      \
	ptr[stride * 0] = r1 >> 24;                             \
	ptr[stride * 1] = r1 >> 16;                             \
	ptr[stride * 2] = r1 >> 8;                              \
	ptr[stride * 3] = r1 >> 0;                              \
	ptr[stride * 4] = r0 >> 24;                             \
	ptr[stride * 5] = r0 >> 16;                             \
	ptr[stride * 6] = r0 >> 8;                              \
	ptr[stride * 7] = r0 >> 0

/* *********************************************************** */

#else /* _LITTLE_ENDIAN */

#define	TRANSP_0(val, shift)                                    \
	r0 = *(TABLE + 2 * val + 0) >> shift;                   \
	r1 = *(TABLE + 2 * val + 1) >> shift

/* *********************************************************** */

#define	TRANSP_1(val, shift)                                    \
	r0 |= *(TABLE + 2 * val + 0) >> shift;                  \
	r1 |= *(TABLE + 2 * val + 1) >> shift

/* *********************************************************** */

#define	STORE(ptr, stride)                                      \
	ptr[stride * 0] = r0 >> 24;                             \
	ptr[stride * 1] = r0 >> 16;                             \
	ptr[stride * 2] = r0 >> 8;                              \
	ptr[stride * 3] = r0 >> 0;                              \
	ptr[stride * 4] = r1 >> 24;                             \
	ptr[stride * 5] = r1 >> 16;                             \
	ptr[stride * 6] = r1 >> 8;                              \
	ptr[stride * 7] = r1 >> 0

#endif /* _LITTLE_ENDIAN */

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */

#define	STORE_VAR	mlib_u64 r

#define	CLEAR_VAR	r = 0

#define	TRANSP_0(val, shift)	r = TABLE[val] >> shift

#define	TRANSP_1(val, shift)	r |= TABLE[val] >> shift

/* *********************************************************** */

#define	STORE(ptr, stride)                                      \
	ptr[stride * 0] = r >> 56;                              \
	ptr[stride * 1] = r >> 48;                              \
	ptr[stride * 2] = r >> 40;                              \
	ptr[stride * 3] = r >> 32;                              \
	ptr[stride * 4] = r >> 24;                              \
	ptr[stride * 5] = r >> 16;                              \
	ptr[stride * 6] = r >> 8;                               \
	ptr[stride * 7] = r >> 0

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
mlib_ImageFlipMainDiag_BIT_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir)
{
	mlib_s32 src_stride = mlib_ImageGetStride(src);
	mlib_s32 dst_stride = mlib_ImageGetStride(dst);
	mlib_s32 ws = mlib_ImageGetWidth(src);
	mlib_s32 hs = mlib_ImageGetHeight(src);
	mlib_s32 wd = mlib_ImageGetWidth(dst);
	mlib_s32 hd = mlib_ImageGetHeight(dst);
	mlib_s32 sw = (ws < hd) ? ws : hd;
	mlib_s32 sh = (hs < wd) ? hs : wd;
	mlib_s32 dx = ws - hd;
	mlib_s32 dy = hs - wd;
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);
	mlib_s32 dyd = ((dx > 0) ? 0 : (-dx - x_dir) >> 1);
	mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);
	mlib_s32 dxd = ((dy > 0) ? 0 : (-dy - y_dir) >> 1);
	mlib_s32 src_bitoffset = mlib_ImageGetBitOffset(src);
	mlib_s32 dst_bitoffset = mlib_ImageGetBitOffset(dst);
	mlib_u8 *sdata = (mlib_u8 *)mlib_ImageGetData(src);
	mlib_u8 *ddata = (mlib_u8 *)mlib_ImageGetData(dst);

	sdata += (dxs + src_bitoffset) / 8 + src_stride * dys;
	ddata += (dxd + dst_bitoffset) / 8 + dst_stride * dyd;
	src_bitoffset = (src_bitoffset + dxs) & 7;
	dst_bitoffset = (dst_bitoffset + dxd) & 7;

	if (y_dir < 0) {
		sdata += (sh - 1) * src_stride;
		src_stride = -src_stride;
	}

	if (x_dir < 0) {
		ddata += (sw - 1) * dst_stride;
		dst_stride = -dst_stride;
	}

	if (!sh || !sw) {
		return (MLIB_SUCCESS);
	}

	if (src_bitoffset == dst_bitoffset && !dst_bitoffset &&
	    !((sh | sw) & 7)) {
		return mlib_ImageFlipMainDiag_BIT_1_AA(sdata, src_stride, ddata,
		    dst_stride, sh, sw);
	} else {
		return mlib_ImageFlipMainDiag_BIT_1_GE(sdata, src_stride,
		    ddata, dst_stride, sh, sw, src_bitoffset, dst_bitoffset);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageFlipMainDiag_BIT_1_AA(
    const mlib_u8 *sdata,
    mlib_s32 src_stride,
    mlib_u8 *ddata,
    mlib_s32 dst_stride,
    mlib_s32 pix_sh,
    mlib_s32 pix_sw)
{
	mlib_s32 i, j;
	const mlib_u8 *addr0, *addr1, *addr2, *addr3;
	const mlib_u8 *addr4, *addr5, *addr6, *addr7;
	mlib_u8 *da;
	mlib_u8 val0, val1, val2, val3;
	mlib_u8 val4, val5, val6, val7;

	STORE_VAR;

	for (j = 0; j < pix_sh >> 3; j++) {
		addr0 = sdata;
		addr1 = addr0 + src_stride;
		addr2 = addr1 + src_stride;
		addr3 = addr2 + src_stride;
		addr4 = addr3 + src_stride;
		addr5 = addr4 + src_stride;
		addr6 = addr5 + src_stride;
		addr7 = addr6 + src_stride;
		da = ddata;

		val0 = *addr0;
		TRANSP_0(val0, 0);
		val1 = *addr1;
		TRANSP_1(val1, 1);
		val2 = *addr2;
		TRANSP_1(val2, 2);
		val3 = *addr3;
		TRANSP_1(val3, 3);
		val4 = *addr4;
		TRANSP_1(val4, 4);
		val5 = *addr5;
		TRANSP_1(val5, 5);
		val6 = *addr6;
		TRANSP_1(val6, 6);
		val7 = *addr7;
		TRANSP_1(val7, 7);

#if defined(_NO_LONGLONG) && defined(__SUNPRO_C)
#pragma unroll(1)
#pragma nomemorydepend
#endif /* defined(_NO_LONGLONG) && defined(__SUNPRO_C) */
		for (i = 0; i < (pix_sw >> 3) - 1; i++) {

			STORE(da, dst_stride);

			addr0++;
			val0 = *addr0;
			TRANSP_0(val0, 0);
			addr1++;
			val1 = *addr1;
			TRANSP_1(val1, 1);
			addr2++;
			val2 = *addr2;
			TRANSP_1(val2, 2);
			addr3++;
			val3 = *addr3;
			TRANSP_1(val3, 3);
			addr4++;
			val4 = *addr4;
			TRANSP_1(val4, 4);
			addr5++;
			val5 = *addr5;
			TRANSP_1(val5, 5);
			addr6++;
			val6 = *addr6;
			TRANSP_1(val6, 6);
			addr7++;
			val7 = *addr7;
			TRANSP_1(val7, 7);
			da += dst_stride << 3;
		}

		STORE(da, dst_stride);

		sdata += src_stride << 3;
		ddata++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageFlipMainDiag_BIT_1_GE(
    const mlib_u8 *sdata,
    mlib_s32 src_stride,
    mlib_u8 *ddata,
    mlib_s32 dst_stride,
    mlib_s32 pix_sh,
    mlib_s32 pix_sw,
    mlib_s32 src_bitoffset,
    mlib_s32 dst_bitoffset)
{
	mlib_d64 BUFFER[BUF_SIZE];
	mlib_d64 *buffer = BUFFER;
	mlib_s32 i, j;
	mlib_u8 *da;
	const mlib_u8 *addr0, *addr1, *addr2, *addr3;
	const mlib_u8 *addr4, *addr5, *addr6, *addr7;
	mlib_u8 val0, val1, val2, val3;
	mlib_u8 val4, val5, val6, val7;
	mlib_s32 pix_sw_byte = (pix_sw + src_bitoffset + 7) / 8;
	mlib_s32 pix_sh_byte = (pix_sh + 7) / 8;
	mlib_s32 src_bitoffset_wr = 0;
	mlib_s32 lim_bg, lim_en;
	mlib_u8 *buff_array[8];

	STORE_VAR;

	if ((mlib_s32)sizeof (BUFFER) < pix_sh_byte) {
		buffer = __mlib_malloc(pix_sh_byte * 8);

		if (!buffer)
			return (MLIB_FAILURE);
	}

	buff_array[0] = (mlib_u8 *)buffer;
	buff_array[1] = buff_array[0] + pix_sh_byte;
	buff_array[2] = buff_array[1] + pix_sh_byte;
	buff_array[3] = buff_array[2] + pix_sh_byte;
	buff_array[4] = buff_array[3] + pix_sh_byte;
	buff_array[5] = buff_array[4] + pix_sh_byte;
	buff_array[6] = buff_array[5] + pix_sh_byte;
	buff_array[7] = buff_array[6] + pix_sh_byte;

	for (j = 0; j < pix_sw_byte; j++) {
		addr0 = sdata + j;
		addr1 = addr0 + src_stride;
		addr2 = addr1 + src_stride;
		addr3 = addr2 + src_stride;
		addr4 = addr3 + src_stride;
		addr5 = addr4 + src_stride;
		addr6 = addr5 + src_stride;
		addr7 = addr6 + src_stride;

		da = (mlib_u8 *)buffer;

#if defined(_NO_LONGLONG) && defined(__SUNPRO_C)
#pragma pipeloop(0)
#pragma unroll(1)
#endif /* defined(_NO_LONGLONG) && defined(__SUNPRO_C) */
		for (i = 0; i < (pix_sh >> 3); i++) {

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

			val0 = *addr0;
			TRANSP_0(val0, 0);
			addr0 += src_stride << 3;
			val1 = *addr1;
			TRANSP_1(val1, 1);
			addr1 += src_stride << 3;
			val2 = *addr2;
			TRANSP_1(val2, 2);
			addr2 += src_stride << 3;
			val3 = *addr3;
			TRANSP_1(val3, 3);
			addr3 += src_stride << 3;
			val4 = *addr4;
			TRANSP_1(val4, 4);
			addr4 += src_stride << 3;
			val5 = *addr5;
			TRANSP_1(val5, 5);
			addr5 += src_stride << 3;
			val6 = *addr6;
			TRANSP_1(val6, 6);
			addr6 += src_stride << 3;
			val7 = *addr7;
			TRANSP_1(val7, 7);
			addr7 += src_stride << 3;

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */

			val0 = *addr0;
			addr0 += src_stride << 3;
			val1 = *addr1;
			addr1 += src_stride << 3;
			val2 = *addr2;
			addr2 += src_stride << 3;
			val3 = *addr3;
			addr3 += src_stride << 3;
			val4 = *addr4;
			addr4 += src_stride << 3;
			val5 = *addr5;
			addr5 += src_stride << 3;
			val6 = *addr6;
			addr6 += src_stride << 3;
			val7 = *addr7;
			addr7 += src_stride << 3;

			TRANSP_0(val0, 0);
			TRANSP_1(val1, 1);
			TRANSP_1(val2, 2);
			TRANSP_1(val3, 3);
			TRANSP_1(val4, 4);
			TRANSP_1(val5, 5);
			TRANSP_1(val6, 6);
			TRANSP_1(val7, 7);

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

			STORE(da, pix_sh_byte);
			da++;
		}

		CLEAR_VAR;
		switch (pix_sh & 7) {
		case 7:
			val6 = *addr6;
			TRANSP_1(val6, 6);
		case 6:
			val5 = *addr5;
			TRANSP_1(val5, 5);
		case 5:
			val4 = *addr4;
			TRANSP_1(val4, 4);
		case 4:
			val3 = *addr3;
			TRANSP_1(val3, 3);
		case 3:
			val2 = *addr2;
			TRANSP_1(val2, 2);
		case 2:
			val1 = *addr1;
			TRANSP_1(val1, 1);
		case 1:
			val0 = *addr0;
			TRANSP_1(val0, 0);
			STORE(da, pix_sh_byte);
			break;
		}

		lim_bg = j ? 0 : src_bitoffset;
		lim_en =
		    (j != pix_sw_byte - 1) ? 8 : (src_bitoffset + pix_sw) & 7;
		lim_en += (lim_en == 0) << 3;

		if (src_bitoffset_wr != dst_bitoffset) {
			for (i = lim_bg; i < lim_en; i++) {
				mlib_ImageCopy_bit_na(buff_array[i], ddata,
				    pix_sh, src_bitoffset_wr, dst_bitoffset);
				ddata += dst_stride;
			}
		} else {
			for (i = lim_bg; i < lim_en; i++) {
				mlib_ImageCopy_bit_al(buff_array[i], ddata,
				    pix_sh, src_bitoffset_wr);
				ddata += dst_stride;
			}
		}
	}

	if (buffer != BUFFER)
		__mlib_free(buffer);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
