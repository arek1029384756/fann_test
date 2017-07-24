######################################################################
# fann_test.pro
######################################################################

TEMPLATE = app
TARGET = fann_test
LOCAL_QT5_PFX = x86_64-linux-gnu
LOCAL_QT5_INC = $$LOCAL_QT5_PFX
LOCAL_QT5_LIB = $$LOCAL_QT5_PFX
INCLUDEPATH += .
INCLUDEPATH += /usr/include/$$LOCAL_QT5_INC/qt5/QtWidgets
QMAKE_CXXFLAGS += -Wextra -pedantic -std=c++11
LIBS += -L/usr/lib/$$LOCAL_QT5_LIB -lQt5Widgets

# Input
HEADERS += FileReader.hpp FileReaderCSV.hpp window.h
SOURCES += main.cpp window.cpp
