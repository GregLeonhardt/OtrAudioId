#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/add/add_api.o \
	${OBJECTDIR}/add/add_lib.o \
	${OBJECTDIR}/create/create_api.o \
	${OBJECTDIR}/create/create_lib.o \
	${OBJECTDIR}/dbase/dbase_api.o \
	${OBJECTDIR}/dbase/dbase_api_episode.o \
	${OBJECTDIR}/dbase/dbase_api_file.o \
	${OBJECTDIR}/dbase/dbase_api_show.o \
	${OBJECTDIR}/dbase/dbase_lib.o \
	${OBJECTDIR}/id3/id3_api.o \
	${OBJECTDIR}/id3/id3_lib.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/mp3/mp3_api.o \
	${OBJECTDIR}/mp3/mp3_lib.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../LibTools/dist/Debug/GNU-Linux/liblibtools.a -lpthread -ldl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/otraudioid

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/otraudioid: ../LibTools/dist/Debug/GNU-Linux/liblibtools.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/otraudioid: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/otraudioid ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/add/add_api.o: add/add_api.c
	${MKDIR} -p ${OBJECTDIR}/add
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/add/add_api.o add/add_api.c

${OBJECTDIR}/add/add_lib.o: add/add_lib.c
	${MKDIR} -p ${OBJECTDIR}/add
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/add/add_lib.o add/add_lib.c

${OBJECTDIR}/create/create_api.o: create/create_api.c
	${MKDIR} -p ${OBJECTDIR}/create
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/create/create_api.o create/create_api.c

${OBJECTDIR}/create/create_lib.o: create/create_lib.c
	${MKDIR} -p ${OBJECTDIR}/create
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/create/create_lib.o create/create_lib.c

${OBJECTDIR}/dbase/dbase_api.o: dbase/dbase_api.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/dbase_api.o dbase/dbase_api.c

${OBJECTDIR}/dbase/dbase_api_episode.o: dbase/dbase_api_episode.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/dbase_api_episode.o dbase/dbase_api_episode.c

${OBJECTDIR}/dbase/dbase_api_file.o: dbase/dbase_api_file.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/dbase_api_file.o dbase/dbase_api_file.c

${OBJECTDIR}/dbase/dbase_api_show.o: dbase/dbase_api_show.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/dbase_api_show.o dbase/dbase_api_show.c

${OBJECTDIR}/dbase/dbase_lib.o: dbase/dbase_lib.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/dbase_lib.o dbase/dbase_lib.c

${OBJECTDIR}/id3/id3_api.o: id3/id3_api.c
	${MKDIR} -p ${OBJECTDIR}/id3
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/id3/id3_api.o id3/id3_api.c

${OBJECTDIR}/id3/id3_lib.o: id3/id3_lib.c
	${MKDIR} -p ${OBJECTDIR}/id3
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/id3/id3_lib.o id3/id3_lib.c

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/mp3/mp3_api.o: mp3/mp3_api.c
	${MKDIR} -p ${OBJECTDIR}/mp3
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mp3/mp3_api.o mp3/mp3_api.c

${OBJECTDIR}/mp3/mp3_lib.o: mp3/mp3_lib.c
	${MKDIR} -p ${OBJECTDIR}/mp3
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mp3/mp3_lib.o mp3/mp3_lib.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
