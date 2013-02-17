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

#ifndef _MLIB_SYS_PROTO_H
#define	_MLIB_SYS_PROTO_H

#pragma ident	"@(#)mlib_sys_proto.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

void *mlib_malloc(
    size_t size);
void *mlib_realloc(
    void *ptr,
    size_t size);
void mlib_free(
    void *ptr);
void *mlib_memset(
    void *s,
    mlib_s32 c,
    size_t n);
void *mlib_memcpy(
    void *s1,
    const void *s2,
    size_t n);
void *mlib_memmove(
    void *s1,
    const void *s2,
    size_t n);

char *mlib_version();

#ifdef __SUNPRO_C
#pragma returns_new_memory(mlib_malloc, mlib_realloc)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SYS_PROTO_H */
