TEMPLATE	     = app
CONFIG		    += qt debug resources
#QMAKE_CXXFLAGS_DEBUG = -g -Wall
QMAKE_LIBS     = -ldl
HEADERS	       = qvv.h   qvv_main_win.h   qvv_view.h   qvv_help.h
SOURCES		     = qvv.cpp qvv_main_win.cpp qvv_view.cpp qvv_help.cpp
FORMS          = qvv_form_confirm_delete.ui
RESOURCES      = qvv.qrc
TARGET		     = qvv

