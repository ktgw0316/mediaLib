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

#pragma ident	"@(#)mlib_c_ImageLookUpMask.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageLookUpMask - table lookup with mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageLookUpMask(mlib_image       *dst,
 *                                       const mlib_image *src,
 *                                       const void       **table,
 *                                       mlib_s32         channels,
 *                                       mlib_s32         cmask)
 *
 * ARGUMENT
 *      dst      Pointer to destination image.
 *      src      Pointer to source image.
 *      table    Lookup table.
 *      channels Number of channels of the lookup table.
 *      cmask    Channel mask - each bit of which represents a channel
 *               in the image. The channels corresponded to 0 bits are not
 *               processed.
 *
 * DESCRIPTION
 *      dst = table[src]
 *
 *  The src and dst can be 1, 2, 3, or 4 channels of MLIB_BYTE, MLIB_SHORT,
 *  MLIB_USHORT or MLIB_INT images. The dst can be of MLIB_FLOAT or
 *  MLIB_DOUBLE image also. The data type of the table must be the same
 *  as that of the dst. The number of entries of the table is
 *  determined by the data type of src.
 *
 *  Only the rightmost dst->channels bits of cmask are considered;
 *
 *  If (src->channels == dst->channels), cmask is for both src and dst;
 *  else, cmask is for dst only;
 *
 *  If (channels == 1), the same table is applied to all channels;
 *  else, there must be channels >= number of valid 1's in cmask;
 *
 *  If ((src->channels == 1) && (dst->channels > 1)),
 *      lookup each channel of the tables with the same source to
 *      obtain the corresponding channel of the destination image;
 *
 *  If ((src->channels != 1) && (src->channels != dst->channels)),
 *      there must be src->channels == number of valid 1's in cmask;
 *
 *  If (channels >= dst->channels), cmask is also applied to the
 *      first (dst->channels) channels of the lookup table;
 *  else, uses the first N channels of the lookup table (where N =
 *      number of valid 1's in cmask);
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLookUpMask.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageLookUpMask = __mlib_ImageLookUpMask

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageLookUpMask) mlib_ImageLookUpMask
    __attribute__((weak, alias("__mlib_ImageLookUpMask")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageLookUpMask(
    mlib_image *dst,
    const mlib_image *src,
    const void **table,
    mlib_s32 channels,
    mlib_s32 cmask)
{
	mlib_s32 slb, dlb, xsize, ysize, dchan, schan, i;
	mlib_type stype, dtype;
	mlib_u8 *sa, *da;
	mlib_s32 table_i[4], dst_i[4], src_i[4], chan = 0;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	schan = mlib_ImageGetChannels(src);
	dchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(src);
	ysize = mlib_ImageGetHeight(src);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	for (i = dchan - 1; i >= 0; i--, cmask >>= 1)
		if ((cmask & 1) == 1) {
			dst_i[chan] = i;
			src_i[chan] = i;
			table_i[chan] = i;
			chan++;
		}

	if (((channels < chan) && (channels != 1)) ||
	    ((schan != dchan) && (schan != 1) && (schan != chan)))
		return (MLIB_FAILURE);

	if (chan == 0)
		return (MLIB_SUCCESS);

	if (chan == dchan) {
		mlib_u8 *look_up_table[4];

		if (channels == 1)
			for (i = 0; i < chan; i++)
				look_up_table[i] = (void *)(table[0]);
		else
			for (i = 0; i < chan; i++)
				look_up_table[i] = (void *)(table[i]);
		return __mlib_ImageLookUp(dst, src,
		    (const void **)look_up_table);
	}

	if (schan == 1)
		for (i = 0; i < chan; i++)
			src_i[i] = 0;
	else if (schan != dchan)
		for (i = 0; i < chan; i++)
			src_i[i] = chan - 1 - i;

	if (channels == 1)
		for (i = 0; i < chan; i++)
			table_i[i] = 0;
	else if (channels < dchan)
		for (i = 0; i < chan; i++)
			table_i[i] = chan - 1 - i;

	if (stype == MLIB_BYTE) {
		if (dtype == MLIB_BYTE) {
			if ((schan == 1) && (chan >= 2) &&
			    ((xsize * ysize) > 250)) {
				if (chan == 2) {
					mlib_ImageLookUpMask_3_4_U8_U8(da, dlb,
					    sa, slb, xsize, ysize,
					    (const mlib_u8 **)table, dst_i,
					    table_i, dchan);
				} else {
					mlib_ImageLookUpMask_4_U8_U8(da, dlb,
					    sa, slb, xsize, ysize,
					    (const mlib_u8 **)table, dst_i,
					    table_i);
				}
			} else {
				mlib_ImageLookUpMask_U8_U8(da, dlb, sa, slb,
				    xsize, ysize, (const mlib_u8 **)table,
				    dst_i, src_i, table_i, dchan, schan, chan);
			}
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {

			if ((schan == 1) && (chan >= 2) &&
			    ((xsize * ysize) > 250)) {
				if (chan == 2) {
					mlib_ImageLookUpMask_3_4_S16_U8(
					    (mlib_s16 *)da, dlb, sa, slb, xsize,
					    ysize, (const mlib_s16 **)table,
					    dst_i, table_i, dchan);
				} else {
					mlib_ImageLookUpMask_4_S16_U8((mlib_s16
					    *)da, dlb, sa, slb, xsize, ysize,
					    (const mlib_s16 **)table, dst_i,
					    table_i);
				}
			} else {
				mlib_ImageLookUpMask_S16_U8(da, dlb, sa, slb,
				    xsize, ysize, (const mlib_s16 **)table,
				    dst_i, src_i, table_i, dchan, schan, chan);
			}
		} else if (dtype == MLIB_INT || dtype == MLIB_FLOAT) {

			if ((schan == 1) && (chan >= 2)) {
				if (chan == 2) {
					mlib_ImageLookUpMask_3_4_S32_U8(
					    (mlib_s32 *)da, dlb, sa, slb, xsize,
					    ysize, (const mlib_s32 **)table,
					    dst_i, table_i, dchan);
				} else {
					mlib_ImageLookUpMask_4_S32_U8((mlib_s32
					    *)da, dlb, sa, slb, xsize, ysize,
					    (const mlib_s32 **)table, dst_i,
					    table_i);
				}
			} else {
				mlib_ImageLookUpMask_S32_U8(da, dlb, sa, slb,
				    xsize, ysize, (const mlib_s32 **)table,
				    dst_i, src_i, table_i, dchan, schan, chan);
			}
		} else if (dtype == MLIB_DOUBLE) {

			if ((schan == 1) && (chan >= 2)) {
				if (chan == 2) {
					mlib_ImageLookUpMask_3_4_D64_U8(
					    (mlib_d64 *)da, dlb, sa, slb, xsize,
					    ysize, (const mlib_d64 **)table,
					    dst_i, table_i, dchan);
				} else {
					mlib_ImageLookUpMask_4_D64_U8((mlib_d64
					    *)da, dlb, sa, slb, xsize, ysize,
					    (const mlib_d64 **)table, dst_i,
					    table_i);
				}
			} else {
				mlib_ImageLookUpMask_D64_U8(da, dlb, sa, slb,
				    xsize, ysize, (const mlib_d64 **)table,
				    dst_i, src_i, table_i, dchan, schan, chan);
			}
		} else
			return (MLIB_FAILURE);
	} else if (stype == MLIB_SHORT) {
		if (dtype == MLIB_BYTE)
			mlib_ImageLookUpMask_U8_S16(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_u8 **)table, dst_i, src_i,
			    table_i, dchan, schan, chan);
		else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT)
			mlib_ImageLookUpMask_S16_S16(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_s16 **)table, dst_i,
			    src_i, table_i, dchan, schan, chan);
		else if (dtype == MLIB_INT || dtype == MLIB_FLOAT)
			mlib_ImageLookUpMask_S32_S16(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_s32 **)table, dst_i,
			    src_i, table_i, dchan, schan, chan);
		else if (dtype == MLIB_DOUBLE)
			mlib_ImageLookUpMask_D64_S16(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_d64 **)table, dst_i,
			    src_i, table_i, dchan, schan, chan);
		else
			return (MLIB_FAILURE);
	} else if (stype == MLIB_USHORT) {
		if (dtype == MLIB_BYTE)
			mlib_ImageLookUpMask_U8_U16(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_u8 **)table, dst_i, src_i,
			    table_i, dchan, schan, chan);
		else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT)
			mlib_ImageLookUpMask_S16_U16(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_s16 **)table, dst_i,
			    src_i, table_i, dchan, schan, chan);
		else if (dtype == MLIB_INT || dtype == MLIB_FLOAT)
			mlib_ImageLookUpMask_S32_U16(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_s32 **)table, dst_i,
			    src_i, table_i, dchan, schan, chan);
		else if (dtype == MLIB_DOUBLE)
			mlib_ImageLookUpMask_D64_U16(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_d64 **)table, dst_i,
			    src_i, table_i, dchan, schan, chan);
		else
			return (MLIB_FAILURE);
	} else if (stype == MLIB_INT) {
		if (dtype == MLIB_BYTE)
			mlib_ImageLookUpMask_U8_S32(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_u8 **)table, dst_i, src_i,
			    table_i, dchan, schan, chan);
		else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT)
			mlib_ImageLookUpMask_S16_S32(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_s16 **)table, dst_i,
			    src_i, table_i, dchan, schan, chan);
		else if (dtype == MLIB_INT || dtype == MLIB_FLOAT)
			mlib_ImageLookUpMask_S32_S32(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_s32 **)table, dst_i,
			    src_i, table_i, dchan, schan, chan);
		else if (dtype == MLIB_DOUBLE)
			mlib_ImageLookUpMask_D64_S32(da, dlb, sa, slb,
			    xsize, ysize, (const mlib_d64 **)table, dst_i,
			    src_i, table_i, dchan, schan, chan);
		else
			return (MLIB_FAILURE);
	} else
		return (MLIB_FAILURE);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
