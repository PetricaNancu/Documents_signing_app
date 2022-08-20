QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS
#TEMPLATE = app
#CONFIG += console c++11
#CONFIG -= app_bundle
#CONFIG -= qt
LIBS += -lws2_32
LIBS += -Advapi32.lib
SOURCES += \
    builders_text.cpp \
    client_connecter.cpp \
    downloader.cpp \
    main.cpp \
    loginner.cpp \
    main_dialog.cpp \
    save_text.cpp \
    text.cpp

HEADERS += \
    builders_text.h \
    client_connecter.h \
    downloader.h \
    loginner.h \
    main_dialog.h \
    save_text.h \
    text.h

FORMS += \
    builders_text.ui \
    downloader.ui \
    loginner.ui \
    main_dialog.ui \
    save_text.ui \
    text.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
