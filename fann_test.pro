######################################################################
# fann_test.pro
######################################################################

TEMPLATE = app
TARGET = fann_test
LOCAL_QT5_PFX = x86_64-linux-gnu

# Qt 5.5.x
# LOCAL_QT5_INC = $$LOCAL_QT5_PFX

# Qt 5.2.x
LOCAL_QT5_INC =

LOCAL_QT5_LIB = $$LOCAL_QT5_PFX
INCLUDEPATH += .
INCLUDEPATH += ./mw ./math ./file_io ./parser ./gui ./gui_qt /usr/include/$$LOCAL_QT5_INC/qt5/QtWidgets
QMAKE_CXXFLAGS += -Wextra -pedantic -std=c++1y
LIBS += -L/usr/lib/$$LOCAL_QT5_LIB -lQt5Widgets

# Input
HEADERS += mw/data_format.hpp \
math/fnn_math.hpp \
file_io/file_reader.hpp \
parser/parser_csv.hpp \
parser/parser_interface.hpp \
gui/gui_graph_interface.hpp \
gui/gui_graph_qt.hpp \
gui/gui_progress_interface.hpp \
gui/gui_progress_qt.hpp \
gui_qt/progress_qt.h \
gui_qt/graph_qt.h

SOURCES += main.cpp \
gui_qt/progress_qt.cpp \
gui_qt/graph_qt.cpp
