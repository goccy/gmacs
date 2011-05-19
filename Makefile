#############################################################################
# Makefile for building: gmacs.app/Contents/MacOS/gmacs
# Generated by qmake (2.01a) (Qt 4.6.2) on: ? 5 ? 20 02:02:46 2011
# Project:  gmacs.pro
# Template: app
# Command: /usr/local/Trolltech/Qt-4.6.2/bin/qmake -macx -o Makefile gmacs.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -gdwarf-2 -arch i386 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -g -gdwarf-2 -arch i386 -Wall -W $(DEFINES)
INCPATH       = -I/usr/local/Trolltech/Qt-4.6.2/mkspecs/macx-g++ -I. -I/usr/local/Trolltech/Qt-4.6.2/lib/QtCore.framework/Versions/4/Headers -I/usr/local/Trolltech/Qt-4.6.2/include/QtCore -I/usr/local/Trolltech/Qt-4.6.2/lib/QtGui.framework/Versions/4/Headers -I/usr/local/Trolltech/Qt-4.6.2/include/QtGui -I/usr/local/Trolltech/Qt-4.6.2/include -I. -I. -F/usr/local/Trolltech/Qt-4.6.2/lib
LINK          = g++
LFLAGS        = -headerpad_max_install_names -arch i386
LIBS          = $(SUBLIBS) -F/usr/local/Trolltech/Qt-4.6.2/lib -L/usr/local/Trolltech/Qt-4.6.2/lib -framework QtGui -L/usr/local/Trolltech/Qt-4.6.2/lib -F/usr/local/Trolltech/Qt-4.6.2/lib -framework QtCore 
AR            = ar cq
RANLIB        = ranlib -s
QMAKE         = /usr/local/Trolltech/Qt-4.6.2/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
export MACOSX_DEPLOYMENT_TARGET = 10.4

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = src/main.cpp 
OBJECTS       = main.o
DIST          = /usr/local/Trolltech/Qt-4.6.2/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/mac.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/mac-g++.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/default_pre.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/dwarf2.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/debug.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/default_post.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/x86.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/objective_c.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/rez.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/sdk.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/lex.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/include_source_dir.prf \
		gmacs.pro
QMAKE_TARGET  = gmacs
DESTDIR       = 
TARGET        = gmacs.app/Contents/MacOS/gmacs

####### Custom Compiler Variables
QMAKE_COMP_QMAKE_OBJECTIVE_CFLAGS = -pipe \
		-g \
		-gdwarf-2 \
		-arch \
		i386 \
		-Wall \
		-W


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile gmacs.app/Contents/PkgInfo gmacs.app/Contents/Resources/empty.lproj gmacs.app/Contents/Info.plist $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) gmacs.app/Contents/MacOS/ || $(MKDIR) gmacs.app/Contents/MacOS/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: gmacs.pro  /usr/local/Trolltech/Qt-4.6.2/mkspecs/macx-g++/qmake.conf /usr/local/Trolltech/Qt-4.6.2/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/mac.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/mac-g++.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/default_pre.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/dwarf2.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/debug.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/default_post.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/x86.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/objective_c.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/rez.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/sdk.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/lex.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/include_source_dir.prf \
		/usr/local/Trolltech/Qt-4.6.2/lib/QtGui.framework/QtGui.prl \
		/usr/local/Trolltech/Qt-4.6.2/lib/QtCore.framework/QtCore.prl
	$(QMAKE) -macx -o Makefile gmacs.pro
/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/unix.conf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/mac.conf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/mac-g++.conf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/qconfig.pri:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_functions.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_config.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/exclusive_builds.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_pre.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/default_pre.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/dwarf2.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/debug.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_post.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/default_post.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/x86.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/objective_c.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/warn_on.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/unix/thread.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/moc.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/rez.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/mac/sdk.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/resources.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/uic.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/yacc.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/lex.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/include_source_dir.prf:
/usr/local/Trolltech/Qt-4.6.2/lib/QtGui.framework/QtGui.prl:
/usr/local/Trolltech/Qt-4.6.2/lib/QtCore.framework/QtCore.prl:
qmake:  FORCE
	@$(QMAKE) -macx -o Makefile gmacs.pro

gmacs.app/Contents/PkgInfo: 
	@$(CHK_DIR_EXISTS) gmacs.app/Contents || $(MKDIR) gmacs.app/Contents 
	@$(DEL_FILE) gmacs.app/Contents/PkgInfo
	@echo "APPL????" >gmacs.app/Contents/PkgInfo
gmacs.app/Contents/Resources/empty.lproj: 
	@$(CHK_DIR_EXISTS) gmacs.app/Contents/Resources || $(MKDIR) gmacs.app/Contents/Resources 
	@touch gmacs.app/Contents/Resources/empty.lproj
	
gmacs.app/Contents/Info.plist: 
	@$(CHK_DIR_EXISTS) gmacs.app/Contents || $(MKDIR) gmacs.app/Contents 
	@$(DEL_FILE) gmacs.app/Contents/Info.plist
	@sed -e "s,@ICON@,,g" -e "s,@EXECUTABLE@,gmacs,g" -e "s,@TYPEINFO@,????,g" /usr/local/Trolltech/Qt-4.6.2/mkspecs/macx-g++/Info.plist.app >gmacs.app/Contents/Info.plist
dist: 
	@$(CHK_DIR_EXISTS) .tmp/gmacs1.0.0 || $(MKDIR) .tmp/gmacs1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/gmacs1.0.0/ && $(COPY_FILE) --parents src/main.cpp .tmp/gmacs1.0.0/ && (cd `dirname .tmp/gmacs1.0.0` && $(TAR) gmacs1.0.0.tar gmacs1.0.0 && $(COMPRESS) gmacs1.0.0.tar) && $(MOVE) `dirname .tmp/gmacs1.0.0`/gmacs1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/gmacs1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) -r gmacs.app
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: src/main.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o src/main.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
