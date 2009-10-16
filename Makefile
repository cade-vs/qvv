#############################################################################
# Makefile for building: qvv
# Generated by qmake (2.01a) (Qt 4.5.1) on: Fri Oct 16 16:15:01 2009
# Project:  qvv.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile qvv.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB
CFLAGS        = -m64 -pipe -g -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -g -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/lib64/qt/mkspecs/linux-g++-64 -I. -I/usr/lib64/qt/include/QtCore -I/usr/lib64/qt/include/QtGui -I/usr/lib64/qt/include -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-rpath,/usr/lib64/qt/lib
LIBS          = $(SUBLIBS)  -L/usr/lib64/qt/lib -ldl -lQtGui -lQtCore -lpthread
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = qvv.cpp \
		qvv_main_win.cpp \
		qvv_view.cpp qrc_qvv.cpp \
		moc_qvv_main_win.cpp \
		moc_qvv_view.cpp
OBJECTS       = qvv.o \
		qvv_main_win.o \
		qvv_view.o \
		qrc_qvv.o \
		moc_qvv_main_win.o \
		moc_qvv_view.o
DIST          = /usr/lib64/qt/mkspecs/common/g++.conf \
		/usr/lib64/qt/mkspecs/common/unix.conf \
		/usr/lib64/qt/mkspecs/common/linux.conf \
		/usr/lib64/qt/mkspecs/qconfig.pri \
		/usr/lib64/qt/mkspecs/features/qt_functions.prf \
		/usr/lib64/qt/mkspecs/features/qt_config.prf \
		/usr/lib64/qt/mkspecs/features/exclusive_builds.prf \
		/usr/lib64/qt/mkspecs/features/default_pre.prf \
		/usr/lib64/qt/mkspecs/features/debug.prf \
		/usr/lib64/qt/mkspecs/features/default_post.prf \
		/usr/lib64/qt/mkspecs/features/resources.prf \
		/usr/lib64/qt/mkspecs/features/qt.prf \
		/usr/lib64/qt/mkspecs/features/unix/thread.prf \
		/usr/lib64/qt/mkspecs/features/moc.prf \
		/usr/lib64/qt/mkspecs/features/include_source_dir.prf \
		qvv.pro
QMAKE_TARGET  = qvv
DESTDIR       = 
TARGET        = qvv

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

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: qvv.pro  /usr/lib64/qt/mkspecs/linux-g++-64/qmake.conf /usr/lib64/qt/mkspecs/common/g++.conf \
		/usr/lib64/qt/mkspecs/common/unix.conf \
		/usr/lib64/qt/mkspecs/common/linux.conf \
		/usr/lib64/qt/mkspecs/qconfig.pri \
		/usr/lib64/qt/mkspecs/features/qt_functions.prf \
		/usr/lib64/qt/mkspecs/features/qt_config.prf \
		/usr/lib64/qt/mkspecs/features/exclusive_builds.prf \
		/usr/lib64/qt/mkspecs/features/default_pre.prf \
		/usr/lib64/qt/mkspecs/features/debug.prf \
		/usr/lib64/qt/mkspecs/features/default_post.prf \
		/usr/lib64/qt/mkspecs/features/resources.prf \
		/usr/lib64/qt/mkspecs/features/qt.prf \
		/usr/lib64/qt/mkspecs/features/unix/thread.prf \
		/usr/lib64/qt/mkspecs/features/moc.prf \
		/usr/lib64/qt/mkspecs/features/include_source_dir.prf
	$(QMAKE) -unix -o Makefile qvv.pro
/usr/lib64/qt/mkspecs/common/g++.conf:
/usr/lib64/qt/mkspecs/common/unix.conf:
/usr/lib64/qt/mkspecs/common/linux.conf:
/usr/lib64/qt/mkspecs/qconfig.pri:
/usr/lib64/qt/mkspecs/features/qt_functions.prf:
/usr/lib64/qt/mkspecs/features/qt_config.prf:
/usr/lib64/qt/mkspecs/features/exclusive_builds.prf:
/usr/lib64/qt/mkspecs/features/default_pre.prf:
/usr/lib64/qt/mkspecs/features/debug.prf:
/usr/lib64/qt/mkspecs/features/default_post.prf:
/usr/lib64/qt/mkspecs/features/resources.prf:
/usr/lib64/qt/mkspecs/features/qt.prf:
/usr/lib64/qt/mkspecs/features/unix/thread.prf:
/usr/lib64/qt/mkspecs/features/moc.prf:
/usr/lib64/qt/mkspecs/features/include_source_dir.prf:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile qvv.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/qvv1.0.0 || $(MKDIR) .tmp/qvv1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/qvv1.0.0/ && $(COPY_FILE) --parents qvv.qrc .tmp/qvv1.0.0/ && $(COPY_FILE) --parents qvv.h qvv_main_win.h qvv_view.h .tmp/qvv1.0.0/ && $(COPY_FILE) --parents qvv.cpp qvv_main_win.cpp qvv_view.cpp .tmp/qvv1.0.0/ && (cd `dirname .tmp/qvv1.0.0` && $(TAR) qvv1.0.0.tar qvv1.0.0 && $(COMPRESS) qvv1.0.0.tar) && $(MOVE) `dirname .tmp/qvv1.0.0`/qvv1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/qvv1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


/usr/lib64/qt/bin/moc:
	(cd $(QTDIR)/src/tools/moc && $(MAKE))

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_rcc_make_all: qrc_qvv.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_qvv.cpp
qrc_qvv.cpp: qvv.qrc \
		qvv_help.html \
		images/go-home.png \
		images/Half_Face_by_uzorpatorica.jpg \
		images/go-up.png \
		images/system-log-out.png \
		images/window-new.png \
		images/Look_To_The_Sun_by_K3win_sm.png \
		images/view-refresh.png
	/usr/lib64/qt/bin/rcc -name qvv qvv.qrc -o qrc_qvv.cpp

compiler_moc_header_make_all: moc_qvv_main_win.cpp moc_qvv_view.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_qvv_main_win.cpp moc_qvv_view.cpp
moc_qvv_main_win.cpp: qvv_main_win.h \
		/usr/lib64/qt/bin/moc
	/usr/lib64/qt/bin/moc $(DEFINES) $(INCPATH) qvv_main_win.h -o moc_qvv_main_win.cpp

moc_qvv_view.cpp: qvv_view.h \
		/usr/lib64/qt/bin/moc
	/usr/lib64/qt/bin/moc $(DEFINES) $(INCPATH) qvv_view.h -o moc_qvv_view.cpp

compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_clean: compiler_rcc_clean compiler_moc_header_clean 

####### Compile

qvv.o: qvv.cpp qvv.h \
		qvv_view.h \
		qvv_main_win.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qvv.o qvv.cpp

qvv_main_win.o: qvv_main_win.cpp qvv.h \
		qvv_view.h \
		qvv_main_win.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qvv_main_win.o qvv_main_win.cpp

qvv_view.o: qvv_view.cpp qvv.h \
		qvv_view.h \
		qvv_main_win.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qvv_view.o qvv_view.cpp

qrc_qvv.o: qrc_qvv.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_qvv.o qrc_qvv.cpp

moc_qvv_main_win.o: moc_qvv_main_win.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_qvv_main_win.o moc_qvv_main_win.cpp

moc_qvv_view.o: moc_qvv_view.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_qvv_view.o moc_qvv_view.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

