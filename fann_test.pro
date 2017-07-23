######################################################################
# fann_test.pro
######################################################################

TEMPLATE = app
TARGET = fann_test
LOCAL_QT5_DIR = x86_64-linux-gnu
INCLUDEPATH += .
INCLUDEPATH += /usr/include/$$LOCAL_QT5_DIR/qt5/QtWidgets
QMAKE_CXXFLAGS += -Wextra -pedantic -std=c++11
LIBS += -L/usr/lib/$$LOCAL_QT5_DIR -lQt5Widgets

# Input
HEADERS += FileReader.hpp FileReaderCSV.hpp window.h
SOURCES += main.cpp window.cpp
