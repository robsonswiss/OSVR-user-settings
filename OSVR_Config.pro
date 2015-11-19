#-------------------------------------------------
#
# Project created by QtCreator 2015-06-04T13:52:08
#
#-------------------------------------------------

QT       += core gui serialport
RC_FILE = myapp.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OSVR_Config
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    osvruser.cpp \
    lib_json/json_reader.cpp \
    lib_json/json_value.cpp \
    lib_json/json_writer.cpp

HEADERS  += mainwindow.h \
    osvruser.h \
    json/assertions.h \
    json/autolink.h \
    json/config.h \
    json/features.h \
    json/forwards.h \
    json/json.h \
    json/reader.h \
    json/value.h \
    json/version.h \
    json/writer.h \
    lib_json/json_batchallocator.h \
    lib_json/json_tool.h

FORMS    += mainwindow.ui

