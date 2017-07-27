######################################################################
# fann_test.pro
######################################################################

TEMPLATE = app
TARGET = fann_test
LOCAL_QT5_PFX = x86_64-linux-gnu
LOCAL_QT5_INC =
LOCAL_QT5_LIB = $$LOCAL_QT5_PFX
INCLUDEPATH += .
INCLUDEPATH += ./file_io ./parser ./gui /usr/include/$$LOCAL_QT5_INC/qt5/QtWidgets
QMAKE_CXXFLAGS += -Wextra -pedantic -std=c++11
LIBS += -L/usr/lib/$$LOCAL_QT5_LIB -lQt5Widgets

# Input
HEADERS += file_io/file_reader.hpp parser/parser_csv.hpp parser/parser_interface.hpp gui/window.h
SOURCES += main.cpp gui/window.cpp
