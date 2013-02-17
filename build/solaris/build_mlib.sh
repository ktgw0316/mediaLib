#! /usr/bin/ksh
#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident "@(#)build_mlib.sh	9.4	08/10/29 SMI"
#

########################################################################

date
uname -a

#
# use major version number for the minute field and minor version number
# for the second field of each file's time stamp
#
VER=2.5.1
VERSION=0251
REV=`date '+%Y.%m.%d.%H.%M'`
TODAY=`date '+%Y''%m%d'`
TIMESTAMP=${TODAY}${VERSION}
PSTAMP=medialib${TIMESTAMP}00

########################################################################

DIR=`dirname $0`

# the current directory
CURDIR=`pwd`

# the base directory of mlib workspace
MLIBBASE=`cd $DIR > /dev/null 2>&1 && pwd | sed -e "s/\/build\/solaris//"`

# the directory to put the distribution
DSTDIR=${MLIBBASE}/dist

# the temporary directories
TMPDIR=${MLIBBASE}/tmp.`uname -p`
OBJDIR=${TMPDIR}/obj

########################################################################

# tools
AR=/usr/ccs/bin/ar
CAT=/usr/bin/cat
CHMOD=/usr/bin/chmod
CP=/usr/bin/cp
DUMP=/usr/ccs/bin/dump
EGREP=/usr/bin/egrep
FIND=/usr/bin/find
GREP=/usr/bin/grep
LD=/usr/ccs/bin/ld
LN=/usr/bin/ln
LS=/usr/bin/ls
MAKE=/usr/ccs/bin/make
MCS=/usr/ccs/bin/mcs
MKDIR=/usr/bin/mkdir
RM=/usr/bin/rm
STRIP="/usr/ccs/bin/strip -x"
TOUCH=/usr/bin/touch

########################################################################

MAPFLAGS="\
	-M ${MLIBBASE}/src/generic/stdc/mapfile			\
	-M ${MLIBBASE}/src/generic/stdc/algebra/mapfile		\
	-M ${MLIBBASE}/src/generic/stdc/algebra/mapfile_common	\
	-M ${MLIBBASE}/src/generic/stdc/graphics/mapfile	\
	-M ${MLIBBASE}/src/generic/stdc/image/mapfile		\
	-M ${MLIBBASE}/src/generic/stdc/image/mapfile_common	\
	-M ${MLIBBASE}/src/generic/stdc/signal/mapfile		\
	-M ${MLIBBASE}/src/generic/stdc/signal/mapfile_common	\
	-M ${MLIBBASE}/src/generic/stdc/video/mapfile		\
	-M ${MLIBBASE}/src/generic/stdc/volume/mapfile		\
"

########################################################################

# clean up when interrupted
trap 'cd ${CURDIR} ; ${RM} -rf ${TMPDIR} ; exit 1' HUP INT KILL TERM

########################################################################

${RM} -rf ${TMPDIR}
${MKDIR} -p ${TMPDIR}

########################################################################

#
# set environment variables for building mediaLib
#
if [ -z "${MLIB_HOME}" ]
then
	MLIB_HOME=${MLIBBASE}
	export MLIB_HOME
fi
if [ -z "${MLIB_CUSTOM}" ]
then
	MLIB_CUSTOM=Solaris
	export MLIB_CUSTOM
fi
if [ -z "${MLIB_CCHOME}" ]
then
	MLIB_CCHOME=/opt/SUNWspro
	export MLIB_CCHOME
fi

echo "MLIB_HOME = ${MLIB_HOME}" 
echo "MLIB_CUSTOM = ${MLIB_CUSTOM}" 
echo "MLIB_CCHOME = ${MLIB_CCHOME}" 

########################################################################

if [ `uname -p` = "sparc" ]
then
	echo "Building sparc libraries ..."

	cd ${MLIBBASE}/src/generic/stdc
	echo "Compiling sparc/v8  ..."
	${MAKE} v8 &
	sleep 10
	echo "Compiling sparc/v8p  ..."
	${MAKE} v8p &
	sleep 10
	echo "Compiling sparc/v9  ..."
	${MAKE} v9 &
	sleep 10

	cd ${MLIBBASE}/src/generic/int
	echo "Compiling sparc_int/v8p  ..."
	env MLIB_CUSTOM=Solaris_int ${MAKE} v8p &
	sleep 10
	echo "Compiling sparc_int/v9  ..."
	env MLIB_CUSTOM=Solaris_int ${MAKE} v9 &
	sleep 10

	cd ${MLIBBASE}/src/sparc/vis
	echo "Compiling sparc/v8pa  ..."
	${MAKE} v8pa &
	sleep 10
	echo "Compiling sparc/v9a  ..."
	${MAKE} v9a &
	sleep 10

	cd ${MLIBBASE}/src/sparc/vis2
	echo "Compiling sparc/v8pb  ..."
	${MAKE} v8pb &
	sleep 10
	echo "Compiling sparc/v9b  ..."
	${MAKE} v9b &

	wait

	echo "Rebuilding sparc shared libraries ..."

	PLATFORMS="sparcv8 sparcv9 sparcv8plus sparcv8plus+vis sparcv9+vis \
		   sparcv8plus+vis2 sparcv9+vis2 sparcv9_filtee"
	INCDIR=${DSTDIR}/sparc/usr/include
	LIBDIR=${DSTDIR}/sparc/usr/lib

	${RM} -rf ${DSTDIR}/sparc
	${MKDIR} -p ${DSTDIR}/sparc

	for platform in ${PLATFORMS}
	do
		# set auxiliary filter and destination directory based on platform
		case ${platform} in
			sparcv8 )
				LDOPTS="-f \$ORIGIN/cpu/\$ISALIST/libmlib.so.2"
				PSRDIR="."
				ARDIR="sparcv8"
				;;
			sparcv9 )
				LDOPTS="-f \$ORIGIN/../cpu/\$ISALIST/sparcv9/libmlib.so.2"
				PSRDIR="sparcv9"
				ARDIR="sparcv9_int"
				;;
			sparcv9_filtee )
				LDOPTS="-z endfiltee"
				PSRDIR="cpu/sparcv9/sparcv9"
				ARDIR="sparcv9_int"
				;;
			sparcv8plus )
				LDOPTS="-z endfiltee"
				PSRDIR="cpu/sparcv8plus"
				ARDIR="sparcv8plus_int"
				;;
			sparcv8plus+vis )
				LDOPTS="-z endfiltee"
				PSRDIR="cpu/sparcv8plus+vis"
				ARDIR="sparcv8plus+vis"
				;;
			sparcv9+vis )
				LDOPTS="-z endfiltee"
				PSRDIR="cpu/sparcv9+vis/sparcv9"
				ARDIR="sparcv9+vis"
				;;
			sparcv8plus+vis2 )
				LDOPTS="-z endfiltee"
				PSRDIR="cpu/sparcv9+vis2"
				ARDIR="sparcv8plus+vis2"
				;;
			sparcv9+vis2 )
				LDOPTS="-z endfiltee"
				PSRDIR="cpu/sparcv9+vis2/sparcv9"
				ARDIR="sparcv9+vis2"
				;;
		esac

		# prepare directories
		cd ${TMPDIR}
		${RM} -rf ${OBJDIR}
		${MKDIR} -p ${OBJDIR}
		if [ ! -d ${LIBDIR}/${PSRDIR} ]
		then
			${MKDIR} -p ${LIBDIR}/${PSRDIR}
		fi

		cd ${OBJDIR}
		if [ "${platform}" != "sparcv9_filtee" ]
		then
			# extract objects from archive
			${AR} -x ${MLIBBASE}/lib/${ARDIR}/libmlib.a
			${STRIP} *.o

			# rebuild shared libraries as auxiliary filters/endfiltees, 
			# which can be verified with the following commands
			#	% dump -Lv libmlib.so.2 | grep AUXILIARY
			#	% dump -Lv libmlib.so.2 | grep ENDFILTEE
			LD_OPTIONS=${LDOPTS} \
			    ${LD} -G -z defs -z text -z combreloc -z lazyload \
			    -B direct -B reduce \
			    ${MAPFLAGS} \
			    -h libmlib.so.2 -o ${LIBDIR}/${PSRDIR}/libmlib.so.2 \
			    *.o -lc -lm
		else
			# extract an object from archive
			${AR} -x ${MLIBBASE}/lib/${ARDIR}/libmlib.a mlib_version.o
			${STRIP} *.o

			# rebuild a shared library as an auxiliary endfiltee, 
			LD_OPTIONS=${LDOPTS} \
			    ${LD} -G -z defs -z text -z combreloc -z lazyload \
			    -B direct -B reduce \
			    -M ${MLIBBASE}/build/solaris/mapfile.sparcv9_filtee \
			    -h libmlib.so.2 -o ${LIBDIR}/${PSRDIR}/libmlib.so.2 \
			    *.o -lc -lm
		fi

		cd ${LIBDIR}/${PSRDIR}
		${STRIP} libmlib.so.2
		${MCS} -d -a "mediaLib:${TIMESTAMP}:${platform}" libmlib.so.2
		${DUMP} -Lv libmlib.so.2 | ${EGREP} 'AUXILIARY|ENDFILTEE'
		${CHMOD} 555 libmlib.so.2

		# clean up
		cd ${TMPDIR}
		${RM} -rf ${OBJDIR}
	done

	echo "Copying header files  ..."
	if [ ! -d ${INCDIR} ]
	then
		${MKDIR} -p ${INCDIR}
	fi
	${CP} -f ${MLIBBASE}/include/* ${INCDIR}

	# time stamp files
	${FIND} ${DSTDIR}/sparc -type d -exec ${CHMOD} 755 '{}' \;
	${FIND} ${DSTDIR}/sparc -type f -exec ${CHMOD} a+r '{}' \;
	${FIND} ${DSTDIR}/sparc -type f -exec ${CHMOD} a-w '{}' \;
	${FIND} ${DSTDIR}/sparc -exec ${TOUCH} -t ${TIMESTAMP} '{}' \;
fi

########################################################################

if [ `uname -p` = "i386" ]
then
	echo "Building x86 libraries ..."

	cd ${MLIBBASE}/src/generic/stdc
	echo "Compiling x86/i386  ..."
	${MAKE} x86 &
	sleep 10
	echo "Compiling x86/amd64  ..."
	${MAKE} amd64 &
	sleep 10

	cd ${MLIBBASE}/src/x86/sse2
	echo "Compiling x86/i386+sse2  ..."
	${MAKE} i386+sse2 &
	sleep 10
	echo "Compiling x86/amd64+sse2  ..."
	${MAKE} amd64+sse2 &

	wait

	echo "Rebuilding x86 shared libraries ..."

	PLATFORMS="i386 amd64 i386+sse2 amd64+sse2"
	INCDIR=${DSTDIR}/x86/usr/include
	LIBDIR=${DSTDIR}/x86/usr/lib
	MAPFLAGS_SSE2="${MAPFLAGS} -M ${MLIBBASE}/build/solaris/mapfile.sse2"

	${RM} -rf ${DSTDIR}/x86
	${MKDIR} -p ${DSTDIR}/x86

	for platform in ${PLATFORMS}
	do
		# set auxiliary filter and destination directory based on platform
		case ${platform} in
			i386 )
				LDOPTS="-f \$ORIGIN/libmlib/\$HWCAP"
				PSRDIR="."
				ARDIR="i386"
				LIBNAME="libmlib.so.2"
				;;
			amd64 )
				LDOPTS="-f \$ORIGIN/../libmlib/amd64/\$HWCAP"
				PSRDIR="amd64"
				ARDIR="amd64"
				LIBNAME="libmlib.so.2"
				;;
			i386+sse2 )
				LDOPTS="-z endfiltee ${MAPFLAGS_SSE2}"
				PSRDIR="libmlib"
				ARDIR="i386+sse2"
				LIBNAME="libmlib_sse2.so.2"
				;;
			amd64+sse2 )
				LDOPTS="-z endfiltee ${MAPFLAGS_SSE2}"
				PSRDIR="libmlib/amd64"
				ARDIR="amd64+sse2"
				LIBNAME="libmlib_sse2.so.2"
				;;
		esac

		# prepare directories
		cd ${TMPDIR}
		${RM} -rf ${OBJDIR}
		${MKDIR} -p ${OBJDIR}
		if [ ! -d ${LIBDIR}/${PSRDIR} ]
		then
			${MKDIR} -p ${LIBDIR}/${PSRDIR}
		fi

		cd ${OBJDIR}
		# extract objects from archive
		${AR} -x ${MLIBBASE}/lib/${ARDIR}/libmlib.a
		${STRIP} *.o

		# rebuild shared libraries as auxiliary filters/endfiltees, 
		# which can be verified with the following commands
		#	% dump -Lv libmlib.so.2 | grep AUXILIARY
		#	% dump -Lv libmlib.so.2 | grep ENDFILTEE
		LD_OPTIONS=${LDOPTS} \
		    ${LD} -G -z defs -z text -z combreloc -z lazyload \
		    -B direct -B reduce \
		    -M /usr/lib/ld/map.noexdata ${MAPFLAGS} \
		    -h ${LIBNAME} -o ${LIBDIR}/${PSRDIR}/${LIBNAME} \
		    *.o -lc -lm

		cd ${LIBDIR}/${PSRDIR}
		${STRIP} ${LIBNAME}
		${MCS} -d -a "mediaLib:${TIMESTAMP}:${platform}" ${LIBNAME}
		${DUMP} -Lv ${LIBNAME} | ${EGREP} 'AUXILIARY|ENDFILTEE'
		${CHMOD} 555 ${LIBNAME}

		# clean up
		cd ${TMPDIR}
		${RM} -rf ${OBJDIR}
	done

	echo "Copying header files  ..."
	if [ ! -d ${INCDIR} ]
	then
		${MKDIR} -p ${INCDIR}
	fi
	${CP} -f ${MLIBBASE}/include/* ${INCDIR}

	# time stamp files
	${FIND} ${DSTDIR}/x86 -type d -exec ${CHMOD} 755 '{}' \;
	${FIND} ${DSTDIR}/x86 -type f -exec ${CHMOD} a+r '{}' \;
	${FIND} ${DSTDIR}/x86 -type f -exec ${CHMOD} a-w '{}' \;
	${FIND} ${DSTDIR}/x86 -exec ${TOUCH} -t ${TIMESTAMP} '{}' \;
fi

########################################################################

# cleanup
cd ${CURDIR}
${RM} -rf ${TMPDIR}

date

########################################################################
