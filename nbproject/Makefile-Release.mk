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
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/2101680177/PauseState.o \
	${OBJECTDIR}/_ext/2101680177/AdvancedOgreFramework.o \
	${OBJECTDIR}/_ext/2101680177/GameState.o \
	${OBJECTDIR}/_ext/2101680177/DotSceneLoader.o \
	${OBJECTDIR}/_ext/2101680177/SoundManager.o \
	${OBJECTDIR}/_ext/2101680177/DemoApp.o \
	${OBJECTDIR}/_ext/2101680177/AppStateManager.o \
	${OBJECTDIR}/_ext/2101680177/main.o \
	${OBJECTDIR}/_ext/2101680177/MenuState.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-mthreads -fmessage-length=0 -fexceptions -fident
CXXFLAGS=-mthreads -fmessage-length=0 -fexceptions -fident

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib/OGRE -Wl,-rpath /home/dahoc/projects/ogrefps/ -lOgreMain -lOIS -lOgreTerrain -lOgrePaging -lOgreRTShaderSystem -lOgreProperty -lCEGUIBase -lfmodex -lBulletCollision -lBulletDynamics -lBulletFileLoader -lBulletMultiThreaded -lBulletSoftBody -lBulletSoftBodySolvers_CPU -lBulletWorldImporter -lConvexDecomposition -lGIMPACTUtils -lLinearMath

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/ogrefps

dist/Release/GNU-Linux-x86/ogrefps: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -Wl,--enable-auto-image-base -Wl,--add-stdcall-alias -Wl,--enable-auto-import -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ogrefps ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/2101680177/PauseState.o: /home/dahoc/projects/ogrefps/src/PauseState.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2101680177
	${RM} $@.d
	$(COMPILE.cc) -O2 -DNDEBUG -I/usr/local/include/OGRE -I/usr/local/include/OIS -I/usr/local/include/boost -I/usr/local/include/fmodex -I/usr/local/include/bullet -Isrc/includes/Collisions/include -Isrc/includes/Dynamics/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2101680177/PauseState.o /home/dahoc/projects/ogrefps/src/PauseState.cpp

${OBJECTDIR}/_ext/2101680177/AdvancedOgreFramework.o: /home/dahoc/projects/ogrefps/src/AdvancedOgreFramework.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2101680177
	${RM} $@.d
	$(COMPILE.cc) -O2 -DNDEBUG -I/usr/local/include/OGRE -I/usr/local/include/OIS -I/usr/local/include/boost -I/usr/local/include/fmodex -I/usr/local/include/bullet -Isrc/includes/Collisions/include -Isrc/includes/Dynamics/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2101680177/AdvancedOgreFramework.o /home/dahoc/projects/ogrefps/src/AdvancedOgreFramework.cpp

${OBJECTDIR}/_ext/2101680177/GameState.o: /home/dahoc/projects/ogrefps/src/GameState.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2101680177
	${RM} $@.d
	$(COMPILE.cc) -O2 -DNDEBUG -I/usr/local/include/OGRE -I/usr/local/include/OIS -I/usr/local/include/boost -I/usr/local/include/fmodex -I/usr/local/include/bullet -Isrc/includes/Collisions/include -Isrc/includes/Dynamics/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2101680177/GameState.o /home/dahoc/projects/ogrefps/src/GameState.cpp

${OBJECTDIR}/_ext/2101680177/DotSceneLoader.o: /home/dahoc/projects/ogrefps/src/DotSceneLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2101680177
	${RM} $@.d
	$(COMPILE.cc) -O2 -DNDEBUG -I/usr/local/include/OGRE -I/usr/local/include/OIS -I/usr/local/include/boost -I/usr/local/include/fmodex -I/usr/local/include/bullet -Isrc/includes/Collisions/include -Isrc/includes/Dynamics/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2101680177/DotSceneLoader.o /home/dahoc/projects/ogrefps/src/DotSceneLoader.cpp

${OBJECTDIR}/_ext/2101680177/SoundManager.o: /home/dahoc/projects/ogrefps/src/SoundManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2101680177
	${RM} $@.d
	$(COMPILE.cc) -O2 -DNDEBUG -I/usr/local/include/OGRE -I/usr/local/include/OIS -I/usr/local/include/boost -I/usr/local/include/fmodex -I/usr/local/include/bullet -Isrc/includes/Collisions/include -Isrc/includes/Dynamics/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2101680177/SoundManager.o /home/dahoc/projects/ogrefps/src/SoundManager.cpp

${OBJECTDIR}/_ext/2101680177/DemoApp.o: /home/dahoc/projects/ogrefps/src/DemoApp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2101680177
	${RM} $@.d
	$(COMPILE.cc) -O2 -DNDEBUG -I/usr/local/include/OGRE -I/usr/local/include/OIS -I/usr/local/include/boost -I/usr/local/include/fmodex -I/usr/local/include/bullet -Isrc/includes/Collisions/include -Isrc/includes/Dynamics/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2101680177/DemoApp.o /home/dahoc/projects/ogrefps/src/DemoApp.cpp

${OBJECTDIR}/_ext/2101680177/AppStateManager.o: /home/dahoc/projects/ogrefps/src/AppStateManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2101680177
	${RM} $@.d
	$(COMPILE.cc) -O2 -DNDEBUG -I/usr/local/include/OGRE -I/usr/local/include/OIS -I/usr/local/include/boost -I/usr/local/include/fmodex -I/usr/local/include/bullet -Isrc/includes/Collisions/include -Isrc/includes/Dynamics/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2101680177/AppStateManager.o /home/dahoc/projects/ogrefps/src/AppStateManager.cpp

${OBJECTDIR}/_ext/2101680177/main.o: /home/dahoc/projects/ogrefps/src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2101680177
	${RM} $@.d
	$(COMPILE.cc) -O2 -DNDEBUG -I/usr/local/include/OGRE -I/usr/local/include/OIS -I/usr/local/include/boost -I/usr/local/include/fmodex -I/usr/local/include/bullet -Isrc/includes/Collisions/include -Isrc/includes/Dynamics/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2101680177/main.o /home/dahoc/projects/ogrefps/src/main.cpp

${OBJECTDIR}/_ext/2101680177/MenuState.o: /home/dahoc/projects/ogrefps/src/MenuState.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2101680177
	${RM} $@.d
	$(COMPILE.cc) -O2 -DNDEBUG -I/usr/local/include/OGRE -I/usr/local/include/OIS -I/usr/local/include/boost -I/usr/local/include/fmodex -I/usr/local/include/bullet -Isrc/includes/Collisions/include -Isrc/includes/Dynamics/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2101680177/MenuState.o /home/dahoc/projects/ogrefps/src/MenuState.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/ogrefps

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
