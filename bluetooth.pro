QT       += core gui
QT +=serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    file.cpp \
    form.cpp \
    main.cpp \
    mainwindow.cpp \
    parse.cpp \
    parse1.cpp \
    send.cpp \
    serial_port.cpp \
    serial_window.cpp \
    serial_window1.cpp \
    setting_text.cpp \
    tables.cpp \
    utils.cpp

HEADERS += \
    file.h \
    form.h \
    mainwindow.h \
    parse.h \
    parse1.h \
    send.h \
    serial_port.h \
    serial_window.h \
    serial_window1.h \
    setting_text.h \
    tables.h \
    utils.h

FORMS += \
    file.ui \
    form.ui \
    mainwindow.ui \
    send.ui \
    serial_window.ui \
    serial_window1.ui \
    setting_text.ui \
    tables.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
