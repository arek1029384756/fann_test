######################################################################
# fann_test.pro
######################################################################

QT += widgets

QMAKE_PROJECT_NAME = fann_test

#LOGMODE = -D_SILENT
LOGMODE = -D_VERBOSE

INCLUDEPATH += .
INCLUDEPATH += ./logger ./mw ./math ./file_io ./parser ./data_processor ./gui ./gui_qt
QMAKE_CXXFLAGS += -Wextra -pedantic -std=c++14 $$LOGMODE

# Input
HEADERS += mw/data_format.hpp \
logger/logger_thread_safe.hpp \
logger/logger.h \
math/fnn_math.hpp \
file_io/file_reader.hpp \
parser/parser_csv.hpp \
parser/parser_interface.hpp \
data_processor/data_processor.hpp \
gui/gui_graph_interface.hpp \
gui/gui_graph_qt.hpp \
gui/gui_progress_interface.hpp \
gui/gui_progress_qt.hpp \
gui_qt/progress_qt.h \
gui_qt/graph_qt.h

SOURCES += main.cpp \
logger/logger.cpp \
gui_qt/progress_qt.cpp \
gui_qt/graph_qt.cpp

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
