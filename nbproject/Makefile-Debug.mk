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
	${OBJECTDIR}/Game.o \
	${OBJECTDIR}/Level.o \
	${OBJECTDIR}/MainMenu.o \
	${OBJECTDIR}/SplashScreen.o \
	${OBJECTDIR}/components/CAnimation.o \
	${OBJECTDIR}/components/CPhysics.o \
	${OBJECTDIR}/components/CPlayerControl.o \
	${OBJECTDIR}/components/CPosition.o \
	${OBJECTDIR}/components/CStaticImage.o \
	${OBJECTDIR}/composition.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++17
CXXFLAGS=-std=c++17

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs sfml-all`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tesimpleplatformer

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tesimpleplatformer: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tesimpleplatformer ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Game.o: Game.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Game.o Game.cpp

${OBJECTDIR}/Level.o: Level.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Level.o Level.cpp

${OBJECTDIR}/MainMenu.o: MainMenu.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MainMenu.o MainMenu.cpp

${OBJECTDIR}/SplashScreen.o: SplashScreen.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SplashScreen.o SplashScreen.cpp

${OBJECTDIR}/components/CAnimation.o: components/CAnimation.cpp
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/components/CAnimation.o components/CAnimation.cpp

${OBJECTDIR}/components/CPhysics.o: components/CPhysics.cpp
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/components/CPhysics.o components/CPhysics.cpp

${OBJECTDIR}/components/CPlayerControl.o: components/CPlayerControl.cpp
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/components/CPlayerControl.o components/CPlayerControl.cpp

${OBJECTDIR}/components/CPosition.o: components/CPosition.cpp
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/components/CPosition.o components/CPosition.cpp

${OBJECTDIR}/components/CStaticImage.o: components/CStaticImage.cpp
	${MKDIR} -p ${OBJECTDIR}/components
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/components/CStaticImage.o components/CStaticImage.cpp

${OBJECTDIR}/composition.o: composition.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/composition.o composition.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

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
